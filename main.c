#include "main.h"
#include "sequence.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include "parallel/mpi.h"

int main(int argc, char** argv)
{

	int ns2 = 0; // number of rna's

	int rank, size;
	time_t start = MPI_Wtime();
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Status status;
	MPI_Datatype SequenceMPIType;
	getDatatype(&SequenceMPIType);
	int sequences_per_proc;
	if (rank == 0)
	{
		FILE* f;
		f = fopen(FILE_NAME, "r");
		if (!f)
		{
			print_error("error loading file.");
			exit(0);
		}

		Sequence* seq = (Sequence*)malloc(sizeof(Sequence));
		read_data_from_file(f, &ns2, &seq);
		fclose(f);
		if (size < 2)
			MPI_Abort(MPI_COMM_WORLD, __LINE__);
		sequences_per_proc = ns2 / (size - 1);

		// 		Telling every Process how many sequences it's about to get
		for (int i = 1; i < size; i++)
			MPI_Send(&sequences_per_proc, 1, MPI_INT, i, TAG, MPI_COMM_WORLD);
		//		Sending each process part of sequences
		for (int i = 0; i * sequences_per_proc < ns2; i++)
		{
			MPI_Send(&seq[i * sequences_per_proc], sequences_per_proc,
				SequenceMPIType, i + 1, TAG, MPI_COMM_WORLD);
		}

		char output[ns2][LINE_LEN];
		//		Gathering answers
		for (int j = 0; j < ns2; j++)
		{
			Sequence received;
			MPI_Recv(&received, 1, SequenceMPIType, MPI_ANY_SOURCE, TAG,
				MPI_COMM_WORLD, &status);
			char str[LINE_LEN];
			snprintf(str, LINE_LEN, "seq %d\tBest offset: Ms(%d) with offset %d\n",
				(received.id), received.best_ms, received.best_offset);
			strcpy(output[received.id], str);
			print_time_diff(start, MPI_Wtime());
		}
		f = fopen(OUTPUT_FILE_NAME, "w");
		for (int i = 0; i < ns2; i++)
			fputs(output[i], f);
		fclose(f);

	}
	else
	{
		MPI_Recv(&sequences_per_proc, 1, MPI_INT, MASTER_RANK, TAG, MPI_COMM_WORLD,
			&status);
		Sequence* sequence = (Sequence*)malloc(
			sizeof(Sequence) * sequences_per_proc);
		MPI_Recv(sequence, sequences_per_proc, SequenceMPIType, MASTER_RANK, TAG,
			MPI_COMM_WORLD, &status);

//#pragma omp parallel for
		for (int i = 0; i < sequences_per_proc; i++)
		{
			printf("Now comparing with DNA no.%d\n", rank);
			get_max_weight_mutant(&sequence[i]);
			MPI_Send(&(sequence[i]), 1, SequenceMPIType, MASTER_RANK, TAG,
				MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();
	return 0;
}



#include "main.h"
#include "sequence.h"
#include "utils.h"
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <string.h>
#include "parallel/mpi.h"

int main(int argc, char** argv)
{

	int ns2 = 0; // number of rna's
	Sequence s;
	int rank, size;
	time_t start = MPI_Wtime();
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Status status;
	MPI_Datatype SequenceMPIType;
	MPI_Datatype type[9] = { MPI_CHAR, MPI_CHAR, MPI_FLOAT, MPI_FLOAT,
		MPI_FLOAT, MPI_FLOAT, MPI_INT, MPI_INT, MPI_INT };
	int blocklen[9] = { MAX_DNA, MAX_RNA, 1, 1, 1, 1, 1, 1, 1 };
	MPI_Aint disp[9];
	disp[0] = (char*)&s.dna - (char*)&s;
	disp[1] = (char*)&s.rna - (char*)&s;
	disp[2] = (char*)&s.w1 - (char*)&s;
	disp[3] = (char*)&s.w2 - (char*)&s;
	disp[4] = (char*)&s.w3 - (char*)&s;
	disp[5] = (char*)&s.w4 - (char*)&s;
	disp[6] = (char*)&s.best_offset - (char*)&s;
	disp[7] = (char*)&s.best_ms - (char*)&s;
	disp[8] = (char*)&s.id - (char*)&s;
	MPI_Type_create_struct(9, blocklen, disp, type, &SequenceMPIType);
	MPI_Type_commit(&SequenceMPIType);
	int sequences_per_proc;
	if (rank == 0)
	{
		int count = 0;
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
			MPI_Abort(MPI_COMM_WORLD, 0);
		sequences_per_proc = ns2 / (size - 1);

		// 		Telling every Process how many sequences it's about to get
		for (int i = 1; i < size; i++)
			MPI_Send(&sequences_per_proc, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		//		Sending each process part of sequences
		for (int i = 0; i * sequences_per_proc < ns2; i++)
		{
			MPI_Send(&seq[i * sequences_per_proc], sequences_per_proc,
				SequenceMPIType, i + 1, 0, MPI_COMM_WORLD);
		}

		char output[ns2][100];
		//		Gathering answers
		for (int j = 0; j < ns2; j++)
		{
			Sequence received;
			printf("waiting...\n");
			MPI_Recv(&received, 1, SequenceMPIType, MPI_ANY_SOURCE, 0,
				MPI_COMM_WORLD, &status);
			char str[100];
			snprintf(str, 100, "seq %d\tBest offset: Ms(%d) with offset %d\n",
				(received.id), received.best_ms, received.best_offset);
			strcpy(output[received.id], str);
			print_time_diff(start, MPI_Wtime());
		}
		f = fopen("output.txt", "w");
		for (int i = 0; i < ns2; i++)
			fputs(output[i], f);
		fclose(f);
		printf("All Sent");

	}
	else
	{
		MPI_Recv(&sequences_per_proc, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
			&status);
		Sequence* sequence = (Sequence*)malloc(
			sizeof(Sequence) * sequences_per_proc);
		MPI_Recv(sequence, sequences_per_proc, SequenceMPIType, 0, 0,
			MPI_COMM_WORLD, &status);
//		printf(
//				"%d recieved\nW1=%.1f\tW2=%.1f\tW3=%.1f\tW4=%.1f\t\n\nMain Sequence:\n%s\n\n",
//				rank, sequence.w1, sequence.w2, sequence.w3, sequence.w4,
//				sequence.rna);
//#pragma omp parallel for
		for (int i = 0; i < sequences_per_proc; i++)
		{
			printf("Now comparing with DNA no.%d\n", rank);
			get_max_weight_mutant(&sequence[i]);
			printf("%d end\n", rank);
			MPI_Send(&(sequence[i]), 1, SequenceMPIType, 0, 0,
				MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();
	return 0;
}


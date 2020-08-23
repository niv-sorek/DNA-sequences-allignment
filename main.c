#include "main.h"
#include "utils.h"
#include <stdlib.h>
#include <time.h>
#include <omp.h>
int main()
{
	time_t start = clock();
	FILE* f;
	float w1, w2, w3, w4;
	int ns2 = 0; // number of semi-sequences
	Sequence seq1;
	Sequence** seq2 = (Sequence**)calloc(1, sizeof(Sequence*));

	omp_set_num_threads(8);
printf("Num of threads %d\n",omp_get_num_threads() );
	f = fopen(FILE_NAME, "r");
	if (!f)
	{
		print_error("error loading file.");
		exit(0);
	}
	read_data_from_file(f, &w1, &w2, &w3, &w4, &ns2, &seq1, seq2);
	fclose(f);
	printf("W1=%.1f\tW2=%.1f\tW3=%.1f\tW4=%.1f\t\n\nMain Sequence:\n%s\n\n", w1, w2, w3, w4, seq1.str);

int i;
#pragma omp for
	for ( i = 0; i < ns2; i++)
	{	int best_ms, best_offset;
		printf("------------------------------------------------------------------\n");
		printf("Now comparing with DNA no.%d\n\n", i);
		get_max_weight_mutant(&seq1, seq2[i], w1, w2, w3, w4, &best_ms, &best_offset);
		printf("Best offset: Ms(%d) with offset %d\n", best_ms, best_offset);
		print_time_diff(start, clock());
	}


	time_t end = clock();
	print_time_diff(start, end);
	return 0;
}



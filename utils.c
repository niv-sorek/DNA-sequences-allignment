//
// Created by sorek on 19/08/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "char_cmp.h"
#include "sequence.h"
/***
 * @details helper function to print errors easily to stderr
 * @param string
 */
void print_error(char* string)
{
	fprintf(stderr, "%s", string);
}
/***
 * @author Niv Sorek
 * @param f txt file
 * @param w1
 * @param w2
 * @param w3
 * @param w4
 */
void read_weights(FILE* f, float* w1, float* w2, float* w3, float* w4)
{
	fscanf(f, "%f %f %f %f\n", w1, w2, w3, w4);
}
/***
 * @author Niv Sorek
 * @param f txt file
 * @param w1
 * @param w2
 * @param w3
 * @param w4
 * @param ns2 number of semi-sequences
 * @param seq1 main sequence
 * @param seq2 array of semi-sequences
 */
void read_data_from_file(FILE* f, int* count, Sequence** sequence)
{
	float w1, w2, w3, w4;
	char dna[MAX_DNA] = { 0 };
	read_weights(f, &w1, &w2, &w3, &w4);
	read_seq(f, dna);
	*count = 0;

	fscanf(f, "%d\n", count);

	*sequence = (Sequence*)realloc(*sequence, *count * sizeof(Sequence));
	if (!*sequence)
	{
		print_error("allocation failed?");
		return;
	}
	if (*count > 0)
	{
		for (int i = 0; i < 4; i++)
		{
//			(*seq1)[i] = (Sequence)malloc(sizeof(Sequence));
//			if (!(*seq1)[i])
//			{
//				print_error("allocation failed!");
//				return;
//			}
			strcpy((*sequence)[i].dna, dna);
			(*sequence)[i].w1 = w1;
			(*sequence)[i].w2 = w2;
			(*sequence)[i].w3 = w3;
			(*sequence)[i].w4 = w4;
			(*sequence)[i].id = i;
			char rna[MAX_RNA] = { 0 };
			read_seq(f, rna);
			strcpy((*sequence)[i].rna, rna);

		}
	}
}

void print_time_diff(double start, double end)
{
	printf("------------------------------------------------------------------\n");
	printf("\tTotal execution time: %f ms  .\n",
		(end - start));
	printf("------------------------------------------------------------------\n");
}


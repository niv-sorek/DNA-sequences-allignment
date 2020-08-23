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
void read_data_from_file(FILE* f, float* w1, float* w2, float* w3, float* w4,
	int* ns2, Sequence* seq1, Sequence** seq2)
{

	read_weights(f, w1, w2, w3, w4);
	read_seq(f, seq1);
	*ns2 = 0;

	fscanf(f, "%d\n", ns2);

	*seq2 = (Sequence*)calloc(sizeof(Sequence), *ns2);
	if (!*seq2)
	{
		print_error("allocation failed");
		return;
	}
	if (*ns2 > 0)
	{
		for (int i = 0; i < *ns2; i++)
		{
			seq2[i] = (Sequence*)malloc(sizeof(Sequence));
			if (!seq2[i])
			{
				print_error("allocation failed");
				return;
			}
			read_seq(f, (seq2[i]));
		}
	}
}


void print_time_diff(double	 start, double end)
{
	printf("------------------------------------------------------------------\n");
	printf("\tTotal execution time: %e ms  .\n",
		(end - start));
	printf("------------------------------------------------------------------\n");
}


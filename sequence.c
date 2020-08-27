//
// Created by sorek on 21/08/2020.
//

#include "char_cmp.h"
#include "sequence.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/***
 *
 * @param ptr
 * @details prints the Sequence and its length
 */
void print_seq(Sequence* ptr)
{
	printf("%s\n", ptr->rna);

}
/***
 * @details reads the next sequence from a text file
 * @param file
 * @param sequence
 */
void read_seq(FILE* file, char* dna)
{

	fgets(dna, MAX_DNA, file);
	dna[strlen(dna) - 1] = '\0';
}
/***
 *
 * @param mutants pointer to array of mutated sequences
 * @param seq sequence to be mutated
 */
void get_mutant_sequences(Sequence** mutants, const Sequence* seq)
{
	*mutants = (Sequence*)malloc(sizeof(Sequence) * strlen(seq->rna));
	if (!mutants)
	{
		print_error("Allocation failed ( tmp in get_mutant_sequences() )");
		return;
	}
//#pragma omp parallel for
	for (int i = 0; i < strlen(seq->rna); i++)
	{
		mutants[i] = (Sequence*)malloc(sizeof(Sequence));
		strcpy(mutants[i]->dna, seq->dna);
		mutants[i]->w1 = seq->w1;
		mutants[i]->w2 = seq->w2;
		mutants[i]->w3 = seq->w3;
		mutants[i]->w4 = seq->w4;
		get_ms(seq, i, mutants[i]->rna);
	}
	printf("%d DNA mutants created\n", (int)strlen(seq->rna));

}
/***
 *
 * @param seq The Sequence to add mutant to
 * @param n index to add '-'
 * @return mutated sequence with '-' at index i
 */
void get_ms(const Sequence* seq, const int n, char mut[MAX_RNA])
{

	for (int i = 0; i < n; i++)
		mut[i] = seq->rna[i];
	for (int i = n; i < MAX_RNA; i++)
		mut[i + 1] = seq->rna[i];
	mut[n] = '-';

}
/***
 *
 * @param s1 first sequence
 * @param s2 second sequence
 * @param offset
 * @param w1
 * @param w2
 * @param w3
 * @param w4
 * @return calculated weight of Comparison between s1 & s2 with offset
 */
float get_sequence_weight(const Sequence* s1, int offset)
{
	float sum = 0;
	//char arr[] = { '*', ':', '.', ' ' };
	int j;
//#pragma omp parallel for   private (j) reduction(+: sum) //NOT HELPING
	for (j = 0; j < strlen(s1->rna) - 1; j++)
	{
		sum += char_compare_weight(s1->dna[j + offset], s1->rna[j], s1->w1, s1->w2, s1->w3, s1->w4);
	}
	return sum;
}

int get_max_weight_offset(const Sequence* seq)
{
	float max_weight = get_sequence_weight(seq, 0);
	int max_offset = 0;
	int limit = (int)strlen(seq->dna) - (int)strlen(seq->rna) + 1;
	float w;
	for (int offset = 1; offset < limit; ++offset)
	{
		w = get_sequence_weight(seq, offset);
		if (w > max_weight)
		{
			max_weight = w;
			max_offset = offset;
		}

	}
	return max_offset;
}
void get_max_weight_mutant(Sequence* const s1)
{
	Sequence** mutants = (Sequence**)calloc(strlen(s1->rna), sizeof(Sequence*));
	get_mutant_sequences(mutants, s1);
	s1->best_offset = get_max_weight_offset(mutants[1]);
	float max_weight = get_sequence_weight(mutants[1], s1->best_offset);

	int w, offset;
//#pragma pmp parallel for
	for (int i = 2; i < strlen(s1->rna) - 1; i++)
	{
		offset = get_max_weight_offset(mutants[i]);
		w = get_sequence_weight(mutants[i], offset);
		if (w > max_weight)
		{
			max_weight = w;
			s1->best_ms = i;
			s1->best_offset = offset;
		}

		free(mutants[i]);
	}

	free(mutants);
}

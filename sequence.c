//
// Created by sorek on 21/08/2020.
//

#include "char_cmp.h"
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
	printf("%s\nsize: %d\n", ptr->str, ptr->len);

}
/***
 * @details reads the next sequence from a text file
 * @param file
 * @param sequence
 */
void read_seq(FILE* file, Sequence* sequence)
{
	char* str = (char*)calloc(1, sizeof(char));
	if (!str)
	{
		print_error("Allocation failed");
		return;
	}
	char c = (char)getc(file);
	sequence->len = 0;
	while (c != '\n' && c != '\0')
	{

		str = (char*)realloc(str, sizeof(char) * (sequence->len + 1));
		if (!str)
		{
			print_error("Allocation failed");
			return;
		}

		str[sequence->len] = c;
		c = (char)getc(file);
		sequence->len++;
	}
	str[sequence->len] = '\0'; // add null-terminator to string
	sequence->str = strdup(str);
	//free(str);
}
/***
 *
 * @param mutants pointer to array of mutated sequences
 * @param seq sequence to be mutated
 */
void get_mutant_sequences(Sequence** mutants, const Sequence* seq)
{
	Sequence* tmp = (Sequence*)malloc(sizeof(Sequence) * seq->len);
	if (!tmp)
	{
		print_error("Allocation failed ( tmp in get_mutant_sequences() )");
		return;
	}
	for (int i = 0; i < seq->len; i++)
	{
		tmp[i].len = seq->len + 1;
		tmp[i].str = (char*)malloc(sizeof(char) * strlen(seq->str));
		if (!tmp[i].str)
		{
			print_error("Allocation failed (tmp[i].str tmp in get_mutant_sequences() )");
			return;
		}
		strcpy(tmp[i].str, get_ms(seq, i));

	}
	printf("%d DNA mutants created\n", seq->len);
	*mutants = tmp;
	free(tmp);
}
/***
 *
 * @param seq The Sequence to add mutant to
 * @param i index to add '-'
 * @return mutated sequence with '-' at index i
 */
char* get_ms(const Sequence* seq, const int i)
{

	char* tmp = (char*)malloc((1 + seq->len) * sizeof(char));
	if (!tmp)
	{ return NULL; }
	strcpy(tmp, seq->str);

	memmove(tmp + i + 1, tmp + i, (strlen(tmp) - i) + 1);
	tmp[i] = '-';
	return tmp;
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
float get_sequence_weight(const Sequence* s1, const Sequence* s2, int offset, float w1, float w2, float w3, float w4)
{
	float sum = 0;
	//char arr[] = { '*', ':', '.', ' ' };

	for (int i = offset, j = 0; i < s1->len && j < s2->len; i++, j++)
	{

	//	printf("%d -%d)\t%c-%c=%c\n", i, j, s1->str[i], s2->str[j], arr[compare_chars(s1->str[i], s2->str[j])]);
		sum += char_compare_weight(s1->str[i], s2->str[j], w1, w2, w3, w4);
	}
	return sum;
}

int get_max_weight_offset(const Sequence* s1, const Sequence* s2, float w1, float w2, float w3, float w4)
{
	float max_weight = get_sequence_weight(s1, s2, 0, w1, w2, w3, w4);
	int max_offset = 0;
	int limit = s1->len - s2->len + 1;
	float w;

	for (int offset = 1; offset < limit; ++offset)
	{
		w = get_sequence_weight(s1, s2, offset, w1, w2, w3, w4);

		if (w > max_weight)
		{
			max_weight = w;
			max_offset = offset;
		}

	}
	return max_offset;
}
void get_max_weight_mutant(const Sequence* s1,
	const Sequence* s2,
	float w1,
	float w2,
	float w3,
	float w4,
	int* best_ms,
	int* best_offset)
{
	Sequence* mutants = (Sequence*)malloc(sizeof(Sequence) * s2->len);
	get_mutant_sequences(&mutants, s2);
	int max_offset = get_max_weight_offset(s1, &mutants[0], w1, w2, w3, w4);
	float max_weight = get_sequence_weight(s1, s2, max_offset, w1, w2, w3, w4);
	int max_i = 0;
	int w, offset;
	for (int i = 1; i < s2->len; i++)
	{
		offset = get_max_weight_offset(s1, &mutants[i], w1, w2, w3, w4);
		w = get_sequence_weight(s1, s2, offset, w1, w2, w3, w4);

		if (w > max_weight)
		{
			max_weight = w;
			max_i = i;
			max_offset = offset;
		}
		free(mutants[i].str);
		//free(&mutants[i]);
	}
	free(mutants);
	*best_ms = max_i;
	*best_offset = max_offset;
}

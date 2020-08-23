//
// Created by sorek on 21/08/2020.
//

#ifndef PARALLEL_FINAL_C_UTILS_C_SEQUENCE_H_
#define PARALLEL_FINAL_C_UTILS_C_SEQUENCE_H_
#include <stdio.h>

typedef struct
{
	char* str;
	int len;
} Sequence;
void print_seq(Sequence* ptr);
void read_seq(FILE* file, Sequence* sequence);
void get_mutant_sequences( Sequence** mutants, const Sequence* seq);
char* get_ms(const Sequence* seq, const int i);
float get_sequence_weight(const Sequence* s1, const Sequence* s2, int offset, float w1, float w2, float w3, float w4);
int get_max_weight_offset (const Sequence *s1, const Sequence *s2, float w1, float w2, float w3, float w4);
void get_max_weight_mutant(const Sequence* s1, const Sequence* s2, float w1, float w2, float w3, float w4, int* best_ms, int* best_offset);


#endif //PARALLEL_FINAL_C_UTILS_C_SEQUENCE_H_

//
// Created by sorek on 21/08/2020.
//

#ifndef PARALLEL_FINAL_C_UTILS_C_SEQUENCE_H_
#define PARALLEL_FINAL_C_UTILS_C_SEQUENCE_H_
#include <stdio.h>
#define MAX_DNA 3000
#define MAX_RNA 2000
typedef struct
{
	char dna[MAX_DNA];
	char rna[MAX_RNA];
	float w1, w2, w3, w4;
	int best_offset, best_ms, id;
} Sequence;
__unused void print_seq(Sequence* ptr);
void read_seq(FILE* file, char* dna);
void get_mutant_sequences(Sequence** mutants, const Sequence* seq);
void get_ms(const Sequence* seq, const int i, char mut[MAX_RNA]);
float get_sequence_weight(const Sequence* s1, int offset);
int get_max_weight_offset(const Sequence* seq);
void get_max_weight_mutant(Sequence* const s1);

#endif //PARALLEL_FINAL_C_UTILS_C_SEQUENCE_H_

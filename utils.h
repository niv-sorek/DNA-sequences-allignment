//
// Created by sorek on 19/08/2020.
//
#include <stdio.h>
#include "main.h"
#include "sequence.h"
#include <time.h>

#ifndef PARALLEL_FINAL_C_UTILS_H
#define PARALLEL_FINAL_C_UTILS_H

#define SEMI_GROUPS_COUNT 11
#define CONSERVATIVE_GROUP_COUNT 9


void print_time_diff(time_t start, time_t end);

void print_error(char *string);

void read_weights(FILE *f, float *w1, float *w2, float *w3, float *w4);

void read_data_from_file(FILE *f, float *w1, float *w2, float *w3, float *w4,
                         int *ns2, Sequence *seq1, Sequence **seq2);

#endif //PARALLEL_FINAL_C_UTILS_H

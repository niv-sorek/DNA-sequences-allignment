//
// Created by sorek on 21/08/2020.
//

#ifndef PARALLEL_FINAL_C_UTILS_C_CHAR_CMP_H_
#define PARALLEL_FINAL_C_UTILS_C_CHAR_CMP_H_
typedef enum
{
	IDENTICAL,
	CONSERVATIVE,
	SEMI,
	DIFFERENT
} Sign;
Sign compare_chars(char a, char b);
int is_conservative(char a, char b);
int is_semi_conservative(char a, char b);
float char_compare_weight(char a, char b, float w1, float w2, float w3, float w4);

#endif //PARALLEL_FINAL_C_UTILS_C_CHAR_CMP_H_

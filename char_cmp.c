//
// Created by sorek on 21/08/2020.
//

#include "utils.h"
#include <string.h>
#include <stdio.h>
#include "char_cmp.h"

int is_semi_conservative (char a, char b)
{
  char *groups[SEMI_GROUPS_COUNT] = {"SAG", "SGND", "NEQHRK", "ATV", "STPA", "NDEQHK", "HFY", "CSA", "STNK", "SNDEQK",
      "FVLIM"};
  int count = 0;
//#pragma omp parallel for
  for (int i = 0; i < SEMI_GROUPS_COUNT; i++)
    {
      if (strchr (groups[i], a) != NULL && strchr (groups[i], b) != NULL)
        count++;
    }
  return count;
}
Sign compare_chars (char a, char b)
{

  if (a == b)
    return IDENTICAL;
  else if (is_conservative (a, b) > 0)
    return CONSERVATIVE;
  else if (is_semi_conservative (a, b) > 0)
    return SEMI;
  return DIFFERENT;
}
int is_conservative (char a, char b)
{
  char *groups[CONSERVATIVE_GROUP_COUNT] = {"NDEQ", "NEQK", "STA", "MILV", "QHRK", "NHQK", "FYW", "HY", "MILF"};
  int count = 0;
  for (int i = 0; i < CONSERVATIVE_GROUP_COUNT; i++)
    {
      if (strchr (groups[i], a) != NULL && strchr (groups[i], b) != NULL)
        count++;
    }
  return count;
}
float char_compare_weight(char a, char b, float w1, float w2, float w3, float w4)
{
	switch (compare_chars (a, b))
	{
	case IDENTICAL: return w1;
	case CONSERVATIVE: return -w2;
	case SEMI: return -w3;
	case DIFFERENT: return -w4;
	default: return 0;
	}
}

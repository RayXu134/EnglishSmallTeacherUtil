#include "util.h"

#include <stdlib.h>
#include <string.h>

int compare_student_names(const void *a, const void *b) {
  char *str_a = *(char **)a;
  char *str_b = *(char **)b;
  int num_a = atoi(str_a);
  int num_b = atoi(str_b);
  if (num_a != num_b) {
    return num_a - num_b;
  }
  return strcmp(str_a, str_b);
}

#include <stdio.h>
#include <assert.h>

#include "est.h"

void usage(char *program) {
  printf("Usage: %s <input_file>\n", program);
  exit(-1);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    usage(argv[0]);
  }

  FILE *input_file = fopen(argv[1], "r");

  if (input_file == NULL) {
    perror("fopen");
    return -1;
  }

  Students *students = NULL;
  Groups *groups = NULL;

  int retval = read_input_file(input_file, &students, &groups);
  if (retval != 0) {
    printf("[Error] Failed running function read_input_file\n");
    return -1;
  }

  retval = calculate_final(students, groups);
  assert(retval == 0);

  int student_count = HASH_COUNT(students);
  Students *sorted[student_count];
  sort_students_by_names(sorted, students);
  for (int i = 0; i < student_count; i++) {
    printf("[Info] (%2d) '%s' in group '%s': %d\n", i, sorted[i]->name, sorted[i]->info.group, sorted[i]->info.point);
  }

  fclose(input_file);
  free_buffer();
  free_students(&students);
  free_groups(&groups);

  return 0;
}

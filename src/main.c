#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "est.h"
#include "util.h"

void usage(char *program) {
  printf("Usage: %s <gen|read> <file>\n", program);
  exit(-1);
}

int func_read(char *filepath) {
  FILE *input_file = fopen(filepath, "r");

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
  free_students(&students);
  free_groups(&groups);

  return 0;
}

int func_gen(char *filepath) {
  printf("[Info] Starting ini file generator\n");

  LineReader line_reader = {0};

  // Check if output file exist.
  FILE *output_file = fopen(filepath, "r");
  if (output_file != NULL) {
    printf("[Error] Please delete file '%s' first\n", filepath);
    fclose(output_file);
    return -1;
  }

  // Create output file.
  output_file = fopen(filepath, "w");
  if (output_file == NULL) {
    perror("fopen");
    return -1;
  }

  char **group_names;

  int student_capacity = 10;
  int student_count = 0;
  char **student_names = malloc(sizeof(char *) * student_capacity);

  // Read groups.

  int group_count;
  printf("Group count: ");
  fflush(stdout);
  if (scanf("%d", &group_count) != 1) {
    perror("scanf");
    return -1;
  }
  if (group_count <= 0) {
    printf("[Error] Please enter a positive integer as group count\n");
    return -1;
  }

  fgetc(stdin); // Take away the remaining \n.

  group_names = malloc(sizeof(char *) * group_count);

  fprintf(output_file, "[Group]\n");

  // Read group names and members.
  for (int i = 0; i < group_count; i++) {
    // Read group name.
    char *line;
    printf("Group %d name: ", i + 1);
    fflush(stdout);
    if ((line = read_line(stdin, &line_reader)) == NULL) {
      printf("[Error] Failed to run function read_line\n");
      return -1;
    }

    // Check length.
    if (strlen(line) == 0) {
      printf("[Error] You must input the group name\n");
      return -1;
    }
    if (strlen(line) > MAX_GROUP_NAME) {
      printf("[Error] MAX_GROUP_NAME (%d) exceeded\n", MAX_GROUP_NAME);
      return -1;
    }

    group_names[i] = strdup(line);

    // Add the group to output file.
    fprintf(output_file, "%s=", group_names[i]);

    // Read group members.
    printf("Enter group %d members (comma seperated): ", i + 1);
    if ((line = read_line(stdin, &line_reader)) == NULL) {
      printf("[Error] Failed to run function read_line\n");
      return -1;
    }

    // Add comma seperated group members to output file.
    fprintf(output_file, "%s\n", line);

    // Parse the comma seperated member list and store student names.
    char delim[] = ",";
    char *name = strtok(line, delim);
    while (name != NULL) {
      while (student_count + 1 > student_capacity) {
        student_capacity *= 2;
        char **temp = realloc(student_names, sizeof(char *) * student_capacity);
        if (temp == NULL) {
          perror("[Error] realloc");
          free(student_names);
          return -1;
        }
        student_names = temp;
      }
      student_names[student_count++] = strdup(name);

      name = strtok(NULL, delim);
    }
  }

  fprintf(output_file, "\n");
  printf("\n");

  // Write template for the GroupPoint namespace.
  printf("[Info] Writing template for the GroupPoint namespace\n");
  fprintf(output_file, "[GroupPoint]\n");
  for (int i = 0; i < group_count; i++) {
    fprintf(output_file, "%s=\n", group_names[i]);
  }
  fprintf(output_file, "\n");

  // Sort student names.
  qsort(student_names, student_count, sizeof(char *), compare_student_names);

  // Write template for the PersonalPoint namespace.
  printf("[Info] Writing template for the PersonalPoint namespace\n");
  fprintf(output_file, "[PersonalPoint]\n");
  for (int i = 0; i < student_count; i++) {
    fprintf(output_file, "%s=\n", student_names[i]);
  }

  // Clean up.

  fclose(output_file);
  line_reader_free(&line_reader);

  for (int i = 0; i < group_count; i++) {
    free(group_names[i]);
  }
  free(group_names);

  for (int i = 0; i < student_count; i++) {
    free(student_names[i]);
  }
  free(student_names);

  return 0;
}

int func_help(char *program_name) {
  usage(program_name);

  // Unreachable.
  return -1;
}

typedef struct {
  char *command;
  int (*func_ptr)(char *);
} Function;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    usage(argv[0]);
  }

  Function functions[] = {
    {"gen", func_gen},
    {"read", func_read},
    {"help", func_help}
  };
  int function_count = sizeof(functions) / sizeof(Function);

  int i;
  for (i = 0; i < function_count; i++) {
    if (strcmp(functions[i].command, argv[1]) == 0) {
      break;
    }
  }

  int retval = -1;
  if (i < function_count) {
    // Command found.
    retval = functions[i].func_ptr(argv[2]);
  } else {
    // Command not found.
    printf("[Info] Bad command, check help\n");
    usage(argv[0]);
  }

  return retval;
}

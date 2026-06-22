#include "est.h"
#include "uthash.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


void free_students(Students **students) {
  Students *el, *tmp;
  HASH_ITER(hh, *students, el, tmp) {
    HASH_DEL(*students, el);
    free(el);
  }
}


void free_groups(Groups **groups) {
  Groups *el, *tmp;
  HASH_ITER(hh, *groups, el, tmp) {
    HASH_DEL(*groups, el);
    free(el->name);
    free(el);
  }
}


size_t buffer_capacity = 0;
size_t buffer_count = 0;
char *buffer = NULL;

// Read the entire line without the trailing '\n'.
char *read_line(FILE *file) {
  buffer_count = 0;

  if (buffer_capacity == 0) {
    buffer_capacity = 128;
    buffer = malloc(buffer_capacity);
    if (buffer == NULL) return NULL;
  }

  int c;
  while ((c = fgetc(file)) != EOF) {
    if (c == '\n') {
      break;
    }

    if (buffer_count + 1 >= buffer_capacity) {
      buffer_capacity *= 2;
      char *new_buffer = realloc(buffer, buffer_capacity);
      if (new_buffer == NULL) {
        free(buffer);
        return NULL;
      }
      buffer = new_buffer;
    }

    buffer[buffer_count++] = c;
  }

  if (buffer_count == 0 && c == EOF) {
    return NULL;
  }

  buffer[buffer_count] = '\0';
  return buffer;
}

typedef enum {
  NAMESPACE_GROUP,
  NAMESPACE_GROUP_POINT,
  NAMESPACE_PERSONAL_POINT,
  NAMESPACE_UNKNOWN
} NamespaceType;

int read_input_file(FILE *file, Students **students, Groups **groups) {
  char *line = NULL;

  NamespaceType current_namespace = NAMESPACE_UNKNOWN;

  while ((line = read_line(file)) != NULL) {
    // Skip empty line.
    if (strlen(line) == 0) continue;
    // Skip comment.
    if (line[0] == '#') continue;

    if (line[0] == '[') {
      if (strlen(line) < 3) {
        // Because namespaces have to use open, close square brackets
        // and name, less than 3 characters can't contain that.
        return -1;
      }

      char *name = line + 1;
      size_t name_len = strcspn(name, "]");

      if (name_len + 2 > strlen(line)) {
        // The length of line is too short that it won't contain
        // the closing square bracket.
        return -1;
      }

      // Cut the name down.
      name[name_len] = '\0';

      if (strcmp(name, "Group") == 0) {
        current_namespace = NAMESPACE_GROUP;
      } else if (strcmp(name, "GroupPoint") == 0) {
        current_namespace = NAMESPACE_GROUP_POINT;
      } else if (strcmp(name, "PersonalPoint") == 0) {
        current_namespace = NAMESPACE_PERSONAL_POINT;
      } else {
        // Unknown namespace.
        current_namespace = NAMESPACE_UNKNOWN;
      }

      continue;
    }

    if (current_namespace == NAMESPACE_UNKNOWN) {
      continue;
    }

    // Try to find the equal sign in the line.
    unsigned long equal_sign = strcspn(line, "=");
    if (equal_sign >= strlen(line)) {
      // Equal sign not found.
      printf("[Error] Bad statement: %s\n", line);
      return -1;
    }

    // Read a key-value pair.
    line[equal_sign] = '\0';
    char *key = line;
    char *value = &(line[equal_sign + 1]);

    // Parse and store the key-value pair.
    if (current_namespace == NAMESPACE_GROUP) {
      // Read group name.
      char *name = key;
      if (strlen(name) > MAX_GROUP_NAME) {
        printf("[Error] MAX_GROUP_NAME (%d) exceeded\n", MAX_GROUP_NAME);
        return -1;
      }

      // Add group.
      Groups *group_to_add = calloc(1, sizeof(Groups));
      group_to_add->name = strdup(name);
      group_to_add->point = 0;

      HASH_ADD_STR(*groups, name, group_to_add);

      // Read group members.
      char delim[] = ",";
      char *student_name = strtok(value, delim);
      while (student_name != NULL) {
        if (strlen(student_name) > MAX_STUDENT_NAME) {
          printf("[Error] MAX_STUDENT_NAME (%d) exceeded\n", MAX_STUDENT_NAME);
        }

        StudentInfo info = {
          .group = group_to_add->name,
          .point = 0
        };

        Students *student_to_add = calloc(1, sizeof(Students));
        strcpy(student_to_add->name, student_name);
        student_to_add->info = info;

        HASH_ADD_STR(*students, name, student_to_add);

        student_name = strtok(NULL, delim);
      }
    } else if (current_namespace == NAMESPACE_GROUP_POINT) {
      char *name = key;

      // Try to find the group.
      Groups *group;
      HASH_FIND_STR(*groups, name, group);
      if (group == NULL) {
        // Group not found.
        printf("[Error] Group not found: '%s'\n", name);
        return -1;
      }

      // Sum group points.
      int total_points = 0;

      char delim[] = ",";
      char *token = strtok(value, delim);
      while (token != NULL) {
        int point;
        if (sscanf(token, "%d", &point) != 1) {
          printf("[Error] Failed to read group points from token: %s\n", token);
        }

        total_points += point;

        token = strtok(NULL, delim);
      }

      // Store total point.
      group->point = total_points;
    } else if (current_namespace == NAMESPACE_PERSONAL_POINT) {
      char *name = key;

      // Try to find the student in students.
      Students *student;
      HASH_FIND_STR(*students, name, student);
      if (student == NULL) {
        // Student name not found.
        printf("[Error] Failed to find the student named: %s\n", name);
        return -1;
      }

      int point;
      if (sscanf(value, "%d", &point) != 1) {
        // Can't read point.
        printf("[Error] Failed to read point from student: %s\n", name);
        return -1;
      }

      student->info.point = point;
    }
  }

  return 0;
}

void free_buffer() {
  free(buffer);
  buffer_capacity = 0;
  buffer_count = 0;
}


int calculate_final(Students *students, Groups *groups) {
  Students *stu, *tmp;
  HASH_ITER(hh, students, stu, tmp) {
    Groups *group;
    HASH_FIND_STR(groups, stu->info.group, group);
    if (group == NULL) {
      // Group not found.
      // This won't happen.
      printf("[Error] UNREACHABLE: Group not found in calculate_final\n");
      return -1;
    }
    stu->info.point += group->point;
  }

  return 0;
}

int compare_students_name(const void *a, const void *b) {
  Students *stuA = *((Students **) a);
  Students *stuB = *((Students **)b);
  if (atoi(stuA->name) != atoi(stuB->name)) return atoi(stuA->name) - atoi(stuB->name);
  return strcmp(stuA->name, stuB->name);
}

int sort_students_by_names(Students *sorted[], Students *students) {
  Students *stu, *tmp;
  int i = 0;
  HASH_ITER(hh, students, stu, tmp) {
    sorted[i] = stu;
    i++;
  }

  int count = HASH_COUNT(students);

  qsort(sorted, count, sizeof(Students *), compare_students_name);

  return 0;
}

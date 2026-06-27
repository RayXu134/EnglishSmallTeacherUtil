#ifndef EST_H_
#define EST_H_

#include <stdio.h>

#include "uthash.h"

// ==============================
// Students
// ==============================

#define MAX_STUDENT_NAME 30

typedef struct {
  char *group;
  int point;
} StudentInfo;

typedef struct {
  char name[MAX_STUDENT_NAME]; // key
  StudentInfo info;
  UT_hash_handle hh;
} Students;

void free_students(Students **students);


// ==============================
// Groups
// ==============================

#define MAX_GROUP_NAME 30

typedef struct {
  char *name;
  int point;
  UT_hash_handle hh;
} Groups;

void free_groups(Groups **groups);


// ==============================
// Line Reader
// ==============================

typedef struct {
  size_t buffer_capacity;
  size_t buffer_count;
  char *buffer;
} LineReader;

char *read_line(FILE *file, LineReader *lr);

void line_reader_free(LineReader *lr);

// ==============================
// Functions
// ==============================

int read_input_file(FILE *file, Students **students, Groups **groups);

int calculate_final(Students *students, Groups *groups);

int sort_students_by_names(Students *sorted[], Students *students);

#endif // EST_H_

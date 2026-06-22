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
// Functions
// ==============================

int read_input_file(FILE *file, Students **students, Groups **groups);

void free_buffer();

int calculate_final(Students *students, Groups *groups);

int sort_students_by_names(Students *sorted[], Students *students);

#endif // EST_H_

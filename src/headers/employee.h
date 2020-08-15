#ifndef __EMP_MAN_EMPLOYEE_H__
#define __EMP_MAN_EMPLOYEE_H__

typedef struct {
  char* id;
  char* first;
  char* last;
  char* email;
  char* address;
  char* phone;
  time_t start;
  char* gender;
  char* ethnicity;
  char* title;
  int* salary;
} employee_t;

typedef struct {
  struct employee_t*      data;
  struct employee_list_node_t* next;
} employee_list_node_t;

typedef struct {
  struct employee_list_node_t* head;
} employee_list_t;

#endif


#include <stdlib.h>
#include <stdio.h>

#include "include/handlers.h"
#include "include/employee.h"

void print_employee(employee_t* item);

void print_employee(employee_t* item) {
  printf("id: %s\n", item->id);
  printf("first name: %s\n", item->first);
  printf("last name: %s\n", item->last);
  printf("email: %s\n", item->email);
  printf("phone: %s\n", item->phone);
  printf("address: %s\n", item->address);
  printf("salary: %d\n", *item->salary);
  printf("ethnicity: %s\n", item->ethnicity);
  printf("title %s\n", item->title);
  printf("gender: %s\n", item->gender);
  printf("start: %ld\n", *item->start);
}

int main(int argc, char** argv) {
  //int a = 20;
  //int b = 10;
  //int c = 5;

  //printf("Calling `empman_rest_handlers_multiply` with a: %d, b: %d\n", a, b);
  //int res = empman_rest_handlers_multiply(a, b);
  //printf("CLIENT - RPC RESPONSE: res = %d\n", res);
  //
  //printf("Calling `empman_rest_handlers_multiply` with a: %d, b: %d\n", c, a);
  //int res_two = empman_rest_handlers_multiply(c, a);
  //printf("CLIENT - RPC RESPONSE: res_two = %d\n", res_two);
  
  char* id = (char*) malloc(sizeof(char) * 33); 
  *id = 'n';
  *(id + 1) = 'i';
  *(id + 2) = 'c';
  *(id + 3) = 'k';

  list_t* employee = empman_rest_handlers_employees_get_id(id);

  serlib_list_iterate(employee, print_employee);

  free(id);

  return 0;
}


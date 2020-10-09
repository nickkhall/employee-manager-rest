#include <stdlib.h>
#include <stdio.h>

#include "include/handlers.h"
#include "include/employee.h"
#include "include/server.h"

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
  empman_rest_handle_traffic();

  //list_t* employee = empman_rest_handlers_employees_get_id(id);

  //serlib_list_iterate(employee, print_employee);

  //free(id);

  return 0;
}


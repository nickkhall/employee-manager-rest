#include <stdlib.h>
#include <stdio.h>

#include "include/handlers.h"
#include "include/employee.h"
#include "include/server.h"

int main(int argc, char** argv) {
  // @TODO: (nickkhall) Implement concurrent requests
  // handle incoming traffic
  server_handle_traffic();

  //list_t* employee = empman_rest_handlers_employees_get_id(id);

  //serlib_list_iterate(employee, print_employee);

  return 0;
}


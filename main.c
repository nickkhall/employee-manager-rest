#include <stdlib.h>
#include <stdio.h>

#include "include/handlers.h"
#include "include/employee.h"
#include "include/server.h"

int main() {
  int socket_type = 3;
  server_init(socket_type);

  return 0;
}


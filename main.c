#include <stdio.h>

#include "src/headers/handlers.h"

int main(int argc, char** argv) {
  int a = 20;
  int b = 10;
  int c = 5;

  printf("Calling `empman_rest_handlers_multiply` with a: %d, b: %d\n", a, b);
  int res = empman_rest_handlers_multiply(a, b);
  printf("CLIENT - RPC RESPONSE: res = %d\n", res);
  
  printf("Calling `empman_rest_handlers_multiply` with a: %d, b: %d\n", c, a);
  int res_two = empman_rest_handlers_multiply(c, a);
  printf("CLIENT - RPC RESPONSE: res_two = %d\n", res_two);

  return 0;
}


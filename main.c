#include <stdio.h>

#include "src/headers/handlers.h"

int main(int argc, char** argv) {
  int a = 20;
  int b = 10;
  int c = 5;

  printf("Calling `multiply_rpc` with a: %d, b: %d\n", a, b);
  int res = multiply_rpc(a, b);
  printf("CLIENT - RPC RESPONSE: res = %d\n", res);
  
  printf("Calling `multiply_rpc` with a: %d, b: %d\n", c, a);
  int res_two = multiply_rpc(c, a);
  printf("CLIENT - RPC RESPONSE: res_two = %d\n", res_two);

  return 0;
}


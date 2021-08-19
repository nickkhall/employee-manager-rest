#include <stdlib.h>
#include <stdio.h>

#include "../include/utils.h"

cJSON* parseRequest(char* request, int request_size) {
  cJSON* data = (cJSON*) malloc(sizeof(cJSON));

  printf("data: %s\ndata size: %d\n", request, request_size);
};

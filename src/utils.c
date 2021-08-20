#include <stdlib.h>
#include <stdio.h>

#include "../include/utils.h"

cJSON* parseRequest(char* request, int request_size) {
  cJSON* data = (cJSON*) malloc(sizeof(cJSON));
  char* buffer_data = (char*) malloc(sizeof(char) * (request_size + 1));

  printf("data: %s\ndata size: %d\n", request, request_size);
  char* request_method = (char*) malloc(sizeof(char) * 10);

  int i = 0;
  while(request[i] != 32) {
    request_method[i] = request[i];
    i++;
  }

  request[i + 1] = '\n';

  printf("request method: %s", request_method);

  free(request_method);
  free(buffer_data);
  return data;
};

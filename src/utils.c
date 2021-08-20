#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/utils.h"

char* parse_request_type(char* request, int request_size) {
  char* request_method = (char*) malloc(sizeof(char) * 10);

  int i = 0;
  while(request[i] != 32) {
    request_method[i] = request[i];
    i++;
  }

  request[i + 1] = '\n';

  return request_method;
};

char* parse_request_url(const char* request) {
    /* Find out where everything is */
    const char *start_of_path = strchr(request, '\n') + 1;
    const char *end_of_path = strchr(start_of_path, '\n') + 1;
    // GET /employees HTTP/1.1

    char* path = (char*) malloc(sizeof(char) * 100);
    strncpy(path, start_of_path, (end_of_path - start_of_path));

    /* Null terminators (because strncpy does not provide them) */
    path[sizeof(path) + 1] = 0;

    /*Print */
    printf("path: %s\n", path);
    return path;
};

cJSON* parse_request(char* request, int request_size) {
  cJSON* data = (cJSON*) malloc(sizeof(cJSON));
  char* buffer_data = (char*) malloc(sizeof(char) * (request_size + 1));

  printf("data: %s\ndata size: %d\n", request, request_size);

  char* request_method = parse_request_type(request, request_size);
  char* request_url = parse_request_url(request);

  printf("request method: %s", request_method);

  if (memcmp(request_method, "GET", 4)) {
    printf("request method: %s", request_method);
  } else if (memcmp(request_method, "POST", 5)) {
    printf("request method: %s", request_method);
  } else if (memcmp(request_method, "PUT", 4)) {
    printf("request method: %s", request_method);
  } 

  free(request_method);
  free(buffer_data);
  return data;
};

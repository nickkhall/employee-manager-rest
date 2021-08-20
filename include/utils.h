#ifndef __REST_UTILS_H__
#define __REST_UTILS_H__

#include "./cJSON.h"

char* parse_request_type(char* request, int request_size);

char* parse_request_url(const char* request);

cJSON* parse_request(char* request, int request_size);

#endif

#ifndef __REST_UTILS_H__
#define __REST_UTILS_H__

#include "./cJSON.h"

cJSON* parseRequest(char* request, int request_size);

#endif

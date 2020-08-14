#ifndef __EMP_MAN_REST_SERIALIZE__
#define __EMP_MAN_REST_SERIALIZE__

#define SERIALIZE_BUFFER_DEFAULT_SIZE 100

typedef struct {
  char* buffer;
  int size;
  int next;
} ser_buff_t;

void emp_man_serialize_init_buffer(ser_buff_t** buffer);

#endif


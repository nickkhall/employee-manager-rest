#ifndef __EMP_MAN_REST_SERIALIZE__
#define __EMP_MAN_REST_SERIALIZE__

#define SERIALIZE_BUFFER_DEFAULT_SIZE 100

typedef struct {
  char* buffer;
  int size;
  int next;
} ser_buff_t;

void serialize_init_buffer(ser_buff_t** b);

void serialize_marshall_data_string(ser_buff_t* b, char* data, int nbytes);

void serialize_demarshall_data_string(char* dest, ser_buff_t* b, int size);

void serialize_buffer_skip(ser_buff_t* b, int skip_size);

void serialize_free_buffer(ser_buff_t* b);

#endif


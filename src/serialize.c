#include <string.h>
#include <stdlib.h>

#include "headers/serialize.h"

/*
 * ------------------------------------------------------
 * function: serialize_init_buffer
 * ------------------------------------------------------
 * params  : buffer - ser_buff_t**
 * ------------------------------------------------------
 * Initializes the serialized buffer type.
 * ------------------------------------------------------
 */
void serialize_init_buffer(ser_buff_t** b) {
  // create memory for serialized buffer type
  (*b) = (ser_buff_t*) calloc(1, sizeof(ser_buff_t));

  // create memory for serialized buffer's buffer
  (*b)->buffer = calloc(1, SERIALIZE_BUFFER_DEFAULT_SIZE);

  // set buffer size to default size
  (*b)->size = SERIALIZE_BUFFER_DEFAULT_SIZE;

  // set buffer's next segment
  (*b)->next = 0;
};

/*
 * ------------------------------------------------------------------------
 * function: serialize_data
 * ------------------------------------------------------------------------
 * params  : 
 *           > buffer  - ser_buff_t**
 *           > data    - char*
 *           > nbytes  - int
 * ------------------------------------------------------------------------
 * Serializes data to a given valid serialized buffer.
 * ------------------------------------------------------------------------
 */
void serialize_marshall_data(ser_buff_t* b, char* data, int nbytes) {
  // get total available size of buffer
  int available_size = b->size - b->next;
  // resize flag used for resizing buffer
  int should_resize = 0;

  // if we don't have enough memory for data in buffer
  while(available_size < nbytes) {
    // increase (multiply) buffer size by 2
    b->size = b->size * 2;

    // update total available size
    available_size = b->size - b->next;

    // set should resize flag
    should_resize = 1;
  }

  // else we have enough memory for data in buffer
  if (should_resize == 0) {
    // copy data from src to buffer's buffer (b->buffer)
    memcpy((char*)b->buffer + b->next, data, nbytes);

    // increase the buffers next memory to nbytes
    b->next += nbytes;

    return;
  }

  // resize the buffer
  b->buffer = realloc(b->buffer, b->size);

  // copy data to buffer's buffer (b->buffer)
  memcpy((char*)b->buffer + b->next, data, nbytes);

  // increase buffer's next memory by nbtyes
  b->next += nbytes;

  return;
};

/*
 * ----------------------------------------------------------------------
 * function: deserialize_data
 * ----------------------------------------------------------------------
 * params  :
 *         > dest   - char*
 *         > buffer - ser_buff_t*
 *         > size   - int
 * ----------------------------------------------------------------------
 * Deserializes a buffers' buffer.
 * ----------------------------------------------------------------------
 */
void serialize_demarshall_data(char* dest, ser_buff_t* b, int size) {
  // copy data from dest to buffer
  memcpy(dest, b->buffer + b->next, size);

  // increment the buffer's next pointer
  b->next += size;
};

/*
 * --------------------------------------------------------------------
 * function: serialize_buffer_skip
 * --------------------------------------------------------------------
 * params  :
 *         > buffer    - ser_buff_t*
 *         > skip_size - int
 * --------------------------------------------------------------------
 * Skips a section of memory in the buffer's buffer. 
 * (In/Decrements the next pointer)
 * --------------------------------------------------------------------
 */
void serialize_buffer_skip(ser_buff_t* b, int skip_size) {
  // if the skip_size is above 0,
  // and the buffer has access to the needed memory
  if (b->next + skip_size > 0 &&
      b->next + skip_size < b->size
  ) {
    // skip the buffer
    // (adjust the next pointer)
    b->next += skip_size;
  }
};

void serialize_free_buffer(ser_buff_t* b) {
  free(b);
};

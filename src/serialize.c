#include <string.h>
#include <stdlib.h>

#include "headers/serialize.h"
#include "headers/employee.h"

/*
 * ------------------------------------------------------
 * function: serialize_init_buffer
 * ------------------------------------------------------
 * params  : b - ser_buff_t**
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

/*
 * --------------------------------------------
 * function: serialize_free_buffer
 * --------------------------------------------
 * params  : b - ser_buff_t*
 * --------------------------------------------
 * Frees the memory and destroys a buffer type.
 * --------------------------------------------
 */
void serialize_free_buffer(ser_buff_t* b) {
  free(b);
};

/*
 * ------------------------------------------------------------------------
 * function: serialize_marshall_data_string
 * ------------------------------------------------------------------------
 * params  : 
 *           > b      - ser_buff_t**
 *           > data   - char*
 *           > nbytes - int
 * ------------------------------------------------------------------------
 * Serializes string data to a given valid serialized string buffer.
 * ------------------------------------------------------------------------
 */
void serialize_marshall_data_string(ser_buff_t* b, char* data, int nbytes) {
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
 * function: serialize_unmarshall_data_string
 * ----------------------------------------------------------------------
 * params  :
 *         > dest - char*
 *         > b    - ser_buff_t*
 *         > size - int
 * ----------------------------------------------------------------------
 * Deserializes a buffers' string buffer.
 * ----------------------------------------------------------------------
 */
void serialize_unmarshall_data_string(char* dest, ser_buff_t* b, int size) {
  // copy data from dest to string buffer
  memcpy(dest, b->buffer + b->next, size);

  // increment the buffer's next pointer
  b->next += size;
};

/*
 * ----------------------------------------------------------------------
 * function: serialize_marshall_data_time_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes a buffers' Employee buffer.
 * ----------------------------------------------------------------------
 */
void serialize_marshall_data_time_t(time_t* dest, ser_buff_t*b, int size) {
};

/*
 * ----------------------------------------------------------------------
 * function: serialize_unmarshall_data_time_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes a buffers' Employee buffer.
 * ----------------------------------------------------------------------
 */
void serialize_unmarshall_data_time_t(time_t* dest, ser_buff_t*b, int size) {
};

/*
 * ----------------------------------------------------------------------
 * function: serialize_marshall_data_employee
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes a buffers' Employee buffer.
 * ----------------------------------------------------------------------
 */
void* serialize_marshall_data_employees(ser_buff_t* b) {
  // set sentintal to default
  unsigned int sentinel = 0;

  // unmarshall buffer to check for sentinel
  serialize_unmarshall_data_string((char*)&sentinel, b, sizeof(unsigned int));

  // if this is a sentinel section, return null
  if (sentinel == 0xFFFFFFFF) {
    return NULL;
  }

  serialize_buffer_skip(b, (int)(-1 * sizeof(unsigned int)));

  Employees* employees = calloc(1, sizeof(Employee));
  // person_t* obj = calloc(1, sizeof(person_t));
  // de_serialize_data((char*)obj->name, b, sizeof(char) * 30);
  // de_serialize_data((char*)obj->age, b, sizeof(int));
  // de_serialize_data((char*)obj->weight, b, sizeof(int));
  //
  //
  //serialize_unmarshall_data_string((char*)employee->id, b, sizeof(char) * 33);
  //serialize_unmarshall_data_string((char*)employee->first, b, sizeof(char) * 33);
  //serialize_unmarshall_data_string((char*)employee->last, b, sizeof(char) * 33);
  //serialize_unmarshall_data_string((char*)employee->email, b, sizeof(char) * 33);
  //serialize_unmarshall_data_string((char*)employee->address, b, sizeof(char) * 33);
  //serialize_unmarshall_data_string((char*)employee->phone, b, sizeof(char) * 33);
  //serialize_unmarshall_data_string((char*)employee->start, b, sizeof(char) * 33);
  //serialize_unmarshall_data_string((char*)employee->gender, b, sizeof(char) * 33);
  //serialize_unmarshall_data_string((char*)employee->ethnicity, b, sizeof(char) * 33);
  //serialize_unmarshall_data_string((char*)employee->title, b, sizeof(char) * 33);
  //serialize_unmarshall_data_string((char*)employee->salary, b, sizeof(char) * 33);
};

/*
 * ----------------------------------------------------------------------
 * function: serialize_unmarshall_data_employees
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes a buffers' Employee buffer.
 * ----------------------------------------------------------------------
 */
void serialize_unmarshall_data_employees(ser_buff_t* b) {

};


/*
 * ----------------------------------------------------------------------
 * function: serialize_marshall_data_employee
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes a buffers' Employee buffer.
 * ----------------------------------------------------------------------
 */
void serialize_marshall_data_employee(ser_buff_t* b) {
  // set sentintal to default
  unsigned int sentinel = 0;

  // unmarshall buffer to check for sentinel
  serialize_unmarshall_data_string((char*)&sentinel, b, sizeof(unsigned int));

  // if this is a sentinel section, return null
  if (sentinel == 0xFFFFFFFF) {
    return NULL;
  }

  serialize_buffer_skip(b, (int)(-1 * sizeof(unsigned int)));
};

/*
 * ----------------------------------------------------------------------
 * function: serialize_unmarshall_data_employee
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes a buffers' Employee buffer.
 * ----------------------------------------------------------------------
 */
void serialize_unmarshall_data_employee(ser_buff_t* b) {

};

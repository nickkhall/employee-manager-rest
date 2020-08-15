#include <string.h>
#include <stdlib.h>

#include "headers/serialize.h"
#include "headers/employee.h"

/*
 * ------------------------------------------------------
 * function: serlib_serialize_init_buffer
 * ------------------------------------------------------
 * params  : b - ser_buff_t**
 * ------------------------------------------------------
 * Initializes the serialized buffer type.
 * ------------------------------------------------------
 */
void serlib_serialize_init_buffer(ser_buff_t** b) {
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
 * function: serlib_serialize_buffer_skip
 * --------------------------------------------------------------------
 * params  :
 *         > buffer    - ser_buff_t*
 *         > skip_size - int
 * --------------------------------------------------------------------
 * Skips a section of memory in the buffer's buffer. 
 * (In/Decrements the next pointer)
 * --------------------------------------------------------------------
 */
void serlib_serialize_buffer_skip(ser_buff_t* b, unsigned long int skip_size) {
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
 * function: serlib_serialize_free_buffer
 * --------------------------------------------
 * params  : b - ser_buff_t*
 * --------------------------------------------
 * Frees the memory and destroys a buffer type.
 * --------------------------------------------
 */
void serlib_serialize_free_buffer(ser_buff_t* b) {
  free(b);
};

/*
 * ------------------------------------------------------------------------
 * function: serlib_serialize_data_string
 * ------------------------------------------------------------------------
 * params  : 
 *           > b      - ser_buff_t**
 *           > data   - char*
 *           > nbytes - int
 * ------------------------------------------------------------------------
 * Serializes string data to a given valid serialized string buffer.
 * ------------------------------------------------------------------------
 */
void serlib_serialize_data_string(ser_buff_t* b, char* data, int nbytes) {
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
 * function: serlib_deserialize_data_string
 * ----------------------------------------------------------------------
 * params  :
 *         > dest - char*
 *         > b    - ser_buff_t*
 *         > size - int
 * ----------------------------------------------------------------------
 * Deserializes a buffers' string buffer.
 * ----------------------------------------------------------------------
 */
void serlib_deserialize_data_string(char* dest, ser_buff_t* b, int size) {
  // copy data from dest to string buffer
  memcpy(dest, b->buffer + b->next, size);

  // increment the buffer's next pointer
  b->next += size;
};

/*
 * ----------------------------------------------------------------------
 * function: serlib_serialize_data_time_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes a buffers' employee_t buffer.
 * ----------------------------------------------------------------------
 */
void serlib_serialize_data_time_t(time_t* dest, ser_buff_t*b, int size) {
};

/*
 * ----------------------------------------------------------------------
 * function: serlib_serialize_data_time_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes a buffers' employee_t buffer.
 * ----------------------------------------------------------------------
 */
void serlib_deserialize_data_time_t(time_t* dest, ser_buff_t*b, int size) {
};

/*
 * ----------------------------------------------------------------------
 * function: serlib_serialize_data_employee
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes a buffers' employee_t buffer.
 * ----------------------------------------------------------------------
void serlib_serialize_data_employees(ser_buff_t* b) {
  // set sentintal to default
  unsigned int sentinel = 0;

  // unmarshall buffer to check for sentinel
  serlib_serialize_data_string((char*)&sentinel, b, sizeof(unsigned int));

  // if this is a sentinel section, return null
  if (sentinel == 0xFFFFFFFF) {
    return NULL;
  }

  serlib_serialize_buffer_skip(b, (int)(-1 * sizeof(unsigned int)));

  employee_ts* employees = calloc(1, sizeof(employee_t));
  // person_t* obj = calloc(1, sizeof(person_t));
  // de_serlib_serialize_data((char*)obj->name, b, sizeof(char) * 30);
  // de_serlib_serialize_data((char*)obj->age, b, sizeof(int));
  // de_serlib_serialize_data((char*)obj->weight, b, sizeof(int));
  //
  //
  //serlib_serialize_data_string((char*)employee->id, b, sizeof(char) * 33);
  //serlib_serialize_data_string((char*)employee->first, b, sizeof(char) * 33);
  //serlib_serialize_data_string((char*)employee->last, b, sizeof(char) * 33);
  //serlib_serialize_data_string((char*)employee->email, b, sizeof(char) * 33);
  //serlib_serialize_data_string((char*)employee->address, b, sizeof(char) * 33);
  //serlib_serialize_data_string((char*)employee->phone, b, sizeof(char) * 33);
  //serlib_serialize_data_string((char*)employee->start, b, sizeof(char) * 33);
  //serlib_serialize_data_string((char*)employee->gender, b, sizeof(char) * 33);
  //serlib_serialize_data_string((char*)employee->ethnicity, b, sizeof(char) * 33);
  //serlib_serialize_data_string((char*)employee->title, b, sizeof(char) * 33);
  //serlib_serialize_data_string((char*)employee->salary, b, sizeof(char) * 33);
};

*/

/*
 * ----------------------------------------------------------------------
 * function: serlib_serialize_employee_list_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes a buffers' employee_t buffer.
 * ----------------------------------------------------------------------
 */
void serlib_serialize_employee_list_t(employee_list_t* employee_list, ser_buff_t* b) {
  // if this is a sentinel section, return null
  if (!employee_list) {
    unsigned int sentinel = 0xFFFFFFFF;
    serlib_serialize_data_string(b, (char*)&sentinel, sizeof(unsigned int));
    return;
  }

  serlib_serialize_employee_list_node_t(employee_list->head, b);
};

/*
 * ----------------------------------------------------------------------
 * function: serlib_deserialize_employee_list_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes a employee list.
 * ----------------------------------------------------------------------
 */
employee_list_t* serlib_deserialize_employee_list_t(ser_buff_t* b) {
  // set sentintal to default
  unsigned int sentinel = 0;

  // unmarshall buffer to check for sentinel
  serlib_serialize_data_string((char*)&sentinel, b, sizeof(unsigned int));

  // if this is a sentinel section, return null
  if (sentinel == 0xFFFFFFFF) {
    return NULL;
  }

  employee_list_t* employee_list = calloc(1, sizeof(employee_list_t));
  employee_list->head = serlib_deserialize_employee_list_node_t(b);

  return employee_list;
};

/*
 * ----------------------------------------------------------------------
 * function: serlib_serialize_data_employee
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes a employee list node.
 * ----------------------------------------------------------------------
 */
void serlib_serialize_employee_list_node_t(employee_list_node_t* employee_list_node, ser_buff_t* b) {
  // if this is a sentinel section, return null
  if (!employee_list_node) {
    unsigned int sentinel = 0xFFFFFFFF;
    serlib_serialize_data_string(b, (char*)&sentinel, sizeof(unsigned int));
    return;
  }

  serlib_serialize_employee_t(b);
  serlib_serialize_employee_list_node_t(employee_list_node->next, b);
};

/*
 * ----------------------------------------------------------------------
 * function: serlib_deserialize_employee_list_node_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes a employee list node.
 * ----------------------------------------------------------------------
 */
employee_list_node_t* serlib_deserialize_employee_list_node_t(ser_buff_t* b) {
  // set sentintal to default
  unsigned int sentinel = 0;

  // unmarshall buffer to check for sentinel
  serlib_serialize_data_string((char*)&sentinel, b, sizeof(unsigned int));

  // if this is a sentinel section, return null
  if (sentinel == 0xFFFFFFFF) {
    return NULL;
  }

  employee_list_node_t* employee_list_node = calloc(1, sizeof(employee_list_node_t));
  
  employee_list_node->data = serlib_deserialize_employee_t(b);
  employee_list_node->next = serlib_deserialize_employee_list_node_t(b);

  return employee_list_node;
};

/*
 * ----------------------------------------------------------------------
 * function: serlib_serialize_employee_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes an employee.
 * ----------------------------------------------------------------------
 */
void serlib_serialize_employee_t(employee_t* employee, ser_buff_t* b) {
  // if this is a sentinel section, return null
  if (!employee) {
    unsigned int sentinel = 0xFFFFFFFF;
    serlib_serialize_data_string(b, (char*)&sentinel, sizeof(unsigned int));
    return;
  }

  serlib_serialize_data_string(b, (char*)employee->id,        sizeof(char) * 33);
  serlib_serialize_data_string(b, (char*)employee->first,     sizeof(char) * 51);
  serlib_serialize_data_string(b, (char*)employee->last,      sizeof(char) * 51);
  serlib_serialize_data_string(b, (char*)employee->email,     sizeof(char) * 101);
  serlib_serialize_data_string(b, (char*)employee->address,   sizeof(char) * 76);
  serlib_serialize_data_string(b, (char*)employee->phone,     sizeof(char) * 51);
  serlib_serialize_data_string(b, (char*)employee->start,     sizeof(time_t));
  serlib_serialize_data_string(b, (char*)employee->gender,    sizeof(char) * 7);
  serlib_serialize_data_string(b, (char*)employee->ethnicity, sizeof(char) * 51);
  serlib_serialize_data_string(b, (char*)employee->title,     sizeof(char) * 51);
  serlib_serialize_data_string(b, (char*)employee->salary,    sizeof(int*));
};

/*
 * ----------------------------------------------------------------------
 * function: serlib_deserialize_employee_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes an employee.
 * ----------------------------------------------------------------------
 */
employee_t* serlib_deserialize_employee_t(ser_buff_t* b) {
  unsigned int sentinel = 0;

  serlib_serialize_data_string(b, (char*)&sentinel, sizeof(unsigned int));

  if (sentinel == 0xFFFFFFFF) {
    return NULL;
  }

  serlib_serialize_buffer_skip(b, -1 * sizeof(unsigned long int));

  employee_t* employee = calloc(1, sizeof(employee_t));

  serlib_serialize_data_string(b, (char*)employee->id,        sizeof(char) * 33);
  serlib_serialize_data_string(b, (char*)employee->first,     sizeof(char) * 51);
  serlib_serialize_data_string(b, (char*)employee->last,      sizeof(char) * 51);
  serlib_serialize_data_string(b, (char*)employee->email,     sizeof(char) * 101);
  serlib_serialize_data_string(b, (char*)employee->address,   sizeof(char) * 76);
  serlib_serialize_data_string(b, (char*)employee->phone,     sizeof(char) * 51);
  serlib_serialize_data_string(b, (char*)employee->start,     sizeof(time_t));
  serlib_serialize_data_string(b, (char*)employee->gender,    sizeof(char) * 7);
  serlib_serialize_data_string(b, (char*)employee->ethnicity, sizeof(char) * 51);
  serlib_serialize_data_string(b, (char*)employee->title,     sizeof(char) * 51);
  serlib_serialize_data_string(b, (char*)employee->salary,    sizeof(int*));

  return employee;
};


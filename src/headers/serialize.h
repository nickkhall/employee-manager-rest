#ifndef __EMP_MAN_REST_SERIALIZE__
#define __EMP_MAN_REST_SERIALIZE__

#define SERIALIZE_BUFFER_DEFAULT_SIZE 100

#include "employee.h"

typedef struct {
  char* buffer;
  int size;
  int next;
} ser_buff_t;

/*
 * ------------------------------------------------------
 * function: serlib_serialize_init_buffer
 * ------------------------------------------------------
 * params  : b - ser_buff_t**
 * ------------------------------------------------------
 * Initializes the serialized buffer type.
 * ------------------------------------------------------
 */
void serlib_serialize_init_buffer(ser_buff_t** b);

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
void serlib_serialize_buffer_skip(ser_buff_t* b, unsigned long int skip_size);

/*
 * --------------------------------------------
 * function: serlib_serialize_free_buffer
 * --------------------------------------------
 * params  : b - ser_buff_t*
 * --------------------------------------------
 * Frees the memory and destroys a buffer type.
 * --------------------------------------------
 */
void serlib_serialize_free_buffer(ser_buff_t* b);

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
void serlib_serialize_data_string(ser_buff_t* b, char* data, int nbytes);

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
void serlib_deserialize_data_string(char* dest, ser_buff_t* b, int size);

/*
 * ----------------------------------------------------------------------
 * function: serlib_serialize_data_time_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes a buffers' employee_t buffer.
 * ----------------------------------------------------------------------
 */
void serlib_serialize_data_time_t(time_t* dest, ser_buff_t*b, int size);

/*
 * ----------------------------------------------------------------------
 * function: serlib_serialize_data_time_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes a buffers' employee_t buffer.
 * ----------------------------------------------------------------------
 */
void serlib_serialize_data_time_t(time_t* dest, ser_buff_t*b, int size);

/*
 * ----------------------------------------------------------------------
 * function: serlib_serialize_employe_node_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes an employee node.
 * ----------------------------------------------------------------------
 */
void serlib_serialize_employee_node_t(ser_buff_t* b);

/*
 * ----------------------------------------------------------------------
 * function: serlib_serialize_employee_list_t
 * ----------------------------------------------------------------------
 * params  :
 *         > employee_list - employee_list_t*
 *         > b             - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes an employee list.
 * ----------------------------------------------------------------------
 */
void serlib_serialize_employee_list_t(employee_list_t* employee_list, ser_buff_t* b);

/*
 * ----------------------------------------------------------------------
 * function: serlib_deserialize_employee_list_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes an employee list.
 * ----------------------------------------------------------------------
 */
employee_list_t* serlib_deserialize_employee_list_t(ser_buff_t* b);

/*
 * ----------------------------------------------------------------------
 * function: serlib_serialize_employee_list_node_t
 * ----------------------------------------------------------------------
 * params  : 
 *         > employee_list_node - employee_list_node_t*
 *         > b                  - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes an employee list node.
 * ----------------------------------------------------------------------
 */
void serlib_serialize_employee_list_node_t(employee_list_node_t* employee_list_node, ser_buff_t* b);

/*
 * ----------------------------------------------------------------------
 * function: serlib_deserialize_employee_list_node_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deerializes a employee list node.
 * ----------------------------------------------------------------------
 */
employee_list_node_t* serlib_deserialize_employee_list_node_t(ser_buff_t* b);

/*
 * ----------------------------------------------------------------------
 * function: serlib_serialize_employee_t
 * ----------------------------------------------------------------------
 * params  :
 *         > employee - employee_t*
 *         > b        - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes a employee_t.
 * ----------------------------------------------------------------------
 */
void serlib_serialize_employee_t(employee_t* employee, ser_buff_t* b);

/*
 * ----------------------------------------------------------------------
 * function: serlib_deserialize_employee_t
 * ----------------------------------------------------------------------
 * params  : 
 *         > employee - employee_t*
 *         > b        - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes a employee_t.
 * ----------------------------------------------------------------------
 */
employee_t* serlib_deserialize_employee_t(ser_buff_t* b);

#endif

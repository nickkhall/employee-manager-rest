#ifndef __EMP_MAN_REST_HANDERS__
#define __EMP_MAN_REST_HANDERS__

#include "serialize.h"
#include "employee.h"

/*
 * -------------------------------------------------------------
 *
 * -------------------------------------------------------------
 *
 * -------------------------------------------------------------
 *
 * -------------------------------------------------------------
 */
list_t* empman_rest_handlers_employees_get_id(char* id);

void empman_rest_send_recv(ser_buff_t* client_send_ser_buffer, ser_buff_t* client_recv_ser_buffer);

ser_buff_t* empman_rest_serialize_employees_get_id(char* id);

list_t* empman_rest_deserialize_employees_get_id(ser_buff_t* client_recv_ser_buffer);

int empman_rest_deserialize_multiply(ser_buff_t* client_recv_ser_buffer);

void empman_rest_init();

int empman_rest_handlers_multiply(int a, int b);

employee_t* employees_employee_initialize(employee_t* employee);

/*
 * ----------------------------------------------------------------------
 * function: employees_deserialize_employee_t_wrapper
 * ----------------------------------------------------------------------
 * params  : 
 *         > obj - void*
 *         > b   - ser_buff_t*
 * ----------------------------------------------------------------------
 * Generic wrapper function for deserializing an employee.
 * ----------------------------------------------------------------------
 */
void employees_deserialize_employee_t_wrapper(void* obj, ser_buff_t* b);

/*
 * ----------------------------------------------------------------------
 * function: employees_deserialize_employee_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes an employee.
 * ----------------------------------------------------------------------
 */
employee_t* employees_deserialize_employee_t(list_node_t* data, ser_buff_t* b);

#endif


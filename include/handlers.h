#ifndef __EMP_MAN_REST_HANDERS_H__
#define __EMP_MAN_REST_HANDERS_H__

#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <sockets.h>
#include <serc.h>

#include "../include/handlers.h"
#include "../include/employee.h"

/*
 *
 *
 *
 *
 */
employee_t* employees_employee_initialize(employee_t* employee);

void empman_rest_send_recv(ser_buff_t* client_send_ser_buffer, ser_buff_t* client_recv_ser_buffer);

/*
 * -------------------------------------------------------------
 * function: empman_rest_handlers_employees_get_id
 * -------------------------------------------------------------
 *
 * -------------------------------------------------------------
 *
 * -------------------------------------------------------------
 */
list_t* empman_rest_handlers_employees_get_id(char* id);

/*
 *
 *
 *
 *
 */
employee_t* employees_employee_create(char** data);

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
void employees_deserialize_employee_t_wrapper(void* data, ser_buff_t* b);

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

/*
 *
 *
 *
 */
ser_buff_t* empman_rest_serialize_employees_get_id(char* id);

list_t* empman_rest_deserialize_employees_get_id(ser_buff_t* client_recv_ser_buffer);
 
#endif


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
employee_t* empman_rest_handlers_employees_get_id(char* id);

void empman_rest_send_recv(ser_buff_t* client_send_ser_buffer, ser_buff_t* client_recv_ser_buffer);


ser_buff_t* empman_rest_serialize_employees_get_id(char* id);

          //empman_rest_deserialize_employess_get_id
employee_t* empman_rest_deserialize_employees_get_id(ser_buff_t* client_recv_ser_buffer);

int empman_rest_deserialize_multiply(ser_buff_t* client_recv_ser_buffer);

void empman_rest_init();

int empman_rest_handlers_multiply(int a, int b);

#endif


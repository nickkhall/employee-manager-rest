#ifndef __EMP_MAN_REST_HANDERS__
#define __EMP_MAN_REST_HANDERS__

#include "handlers.h"
#include "serialize.h"

/*
 * -------------------------------------------------------------
 *
 * -------------------------------------------------------------
 *
 * -------------------------------------------------------------
 *
 * -------------------------------------------------------------
 */
employee_t* get_employee(char* id);

void empman_rest_send_recv(ser_buff_t* client_send_ser_buffer, ser_buff_t* client_recv_ser_buffer);


ser_buff_t* empman_rest_serialize_multiply(int a, int b);

int empman_rest_deserialize_multiply(ser_buff_t* client_recv_ser_buffer);

void empman_rest_init();

int empman_rest_handlers_multiply(int a, int b);

#endif


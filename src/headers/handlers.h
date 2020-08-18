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

void rpc_send_recv(ser_buff_t* client_send_ser_buffer, ser_buff_t* client_recv_ser_buffer);

ser_buff_t* multiply_client_stub_marshal(int a, int b);

int multiply_client_stub_unmarshal(ser_buff_t* client_recv_ser_buffer);

void init_rpc_infra();

int multiply_rpc(int a, int b);

#endif


#ifndef __EMP_MAN_REST_SERVER_H__
#define __EMP_MAN_REST_SERVER_H__

#include <serialize.h>

int* server_init();

void server_init_buffers(ser_buff_t** recv_buffer, ser_buff_t** send_buffer);

void server_process_traffic(ser_buff_t** recv_buffer, ser_buff_t** send_buffer);

void server_handle_traffic();

#endif


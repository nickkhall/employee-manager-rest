#ifndef __EMP_MAN_REST_SERVER_H__
#define __EMP_MAN_REST_SERVER_H__

#include <serialize.h>

int* server_init(void);

int* server_new_socket(int port);

void server_init_buffers(ser_buff_t** recv_buffer, ser_buff_t** send_buffer);

void server_process_traffic(ser_buff_t** recv_buffer, ser_buff_t** send_buffer);

void server_socket_new_thread(int port,
                              ser_buff_t** recv_buffer,
                              ser_buff_t** send_buffer,
                              struct sockaddr* client_addr,
                              socklen_t* addr_len);

#endif


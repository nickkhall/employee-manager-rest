#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <serialize.h>
#include <sockets.h>

#include "../include/common.h"
#include "../include/server.h"

int* server_init() {
  // create socket memory
  int* sock_tcp_fd = (int*) malloc(sizeof(int));
  if (!sock_tcp_fd) {
    printf("ERROR:: REST - Failed to allocate memory for socket in server_init\n");
    return NULL;
  }

  // create socket
  sock_tcp_fd = socklib_socket_create(REST_SERVER_PORT);
  if (*sock_tcp_fd == -1) {
    printf("ERROR:: REST - Failed to create socket in server_init\n");
    exit(1);
  }

  // create server socket
  struct sockaddr_in* server_addr = socklib_socket_build_sock_addr_in(sock_tcp_fd, AF_INET, REST_SERVER_PORT);

  // bind to rest port
  int binded = bind(*sock_tcp_fd, (struct sockaddr*) &*server_addr, sizeof(struct sockaddr));
  if (binded == -1) {
    printf("REST ERROR:: Failed to bind to socket\n");
    exit(1);
  }

  return sock_tcp_fd;
}

void server_init_buffers(ser_buff_t** recv_buffer, ser_buff_t** send_buffer)
{
  serlib_init_buffer_of_size(send_buffer, MAX_RECV_BUFF_SIZE);
  serlib_init_buffer_of_size(recv_buffer, MAX_RECV_BUFF_SIZE);
}


void server_process_traffic(ser_buff_t** recv_buffer, ser_buff_t** send_buffer)
{
  ser_header_t* rest_ser_header = (ser_header_t*) malloc(sizeof(ser_header_t));
  if (!rest_ser_header) {
    printf("ERROR:: REST - Failed to allocate memory for rest_ser_header\n");
    free(recv_buffer);
    free(send_buffer);
    exit(1);
  }

  serlib_deserialize_data(*recv_buffer, (char*)&rest_ser_header->tid,          sizeof(rest_ser_header->tid));
  serlib_deserialize_data(*recv_buffer, (char*)&rest_ser_header->rpc_proc_id,  sizeof(rest_ser_header->rpc_proc_id));
  serlib_deserialize_data(*recv_buffer, (char*)&rest_ser_header->rpc_call_id,  sizeof(rest_ser_header->rpc_call_id));
  serlib_deserialize_data(*recv_buffer, (char*)&rest_ser_header->payload_size, sizeof(rest_ser_header->payload_size));

  // @TODO: update to use `rest_call_id` instead of `rest_proc_id`
  switch (rest_ser_header->rpc_proc_id) {
    default:
      printf("Default switch\n");
      free(recv_buffer);
      free(send_buffer);
      server_handle_traffic();
      break;
  }
}

void server_handle_traffic()
{
  int* sock_tcp_fd = server_init();

  // listen and accept up to 1 HUNDRED connections
  // honey badge dont care, honey badger dont giva shit
  listen(*sock_tcp_fd, 100);
  // print running message to screen
  printf("- Employee Manager - \nREST - Server is now listening on port %d...\n", REST_SERVER_PORT);

  struct sockaddr_in* server_addr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
  if (!server_addr) {
    printf("ERROR:: REST - Failed to allocate memory for server socket to handle traffic\n");
    exit(1);
  }

  struct sockaddr_in client_addr;
  int addr_len = sizeof(struct sockaddr);

  // create and initialize send/recv buffers
  ser_buff_t** recv_buffer = (ser_buff_t**) malloc(MAX_RECV_BUFF_SIZE);
  ser_buff_t** send_buffer = (ser_buff_t**) malloc(MAX_RECV_BUFF_SIZE);
  server_init_buffers(recv_buffer, send_buffer);

  // reset recv buffer
  serlib_reset_buffer(*recv_buffer);

  // accept incoming requests
  accept(*sock_tcp_fd, (struct sockaddr_in*) &client_addr, (socklen_t*) &addr_len);
  
  // receive data from request into local buffer
  int len = recvfrom(*sock_tcp_fd, &(*(*recv_buffer)->buffer),
                     serlib_get_buffer_length(*recv_buffer),
                     0, (struct sockaddr*)&client_addr,
                     (socklen_t*)&addr_len);

  // print status
  printf("REST server recieved %d bytes\n", len);

  // reset send buffer
  serlib_reset_buffer(*send_buffer);

  // process request
  server_process_traffic(recv_buffer, send_buffer);

  // send the serialized result to client
  len = sendto(*sock_tcp_fd, (*send_buffer)->buffer,
              serlib_get_buffer_length(*send_buffer),
              0, (struct sockaddr*)&client_addr,
              sizeof(struct sockaddr));

  // reset send buffer
  serlib_reset_buffer(*send_buffer);

  close(*sock_tcp_fd);
}


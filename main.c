#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#include "src/headers/common.h"
#include "src/headers/serialize.h"
#include "src/headers/sockets.h"

#define MULTIPLY_ID 55

// @TODO: THIS IS A POC FOR LEARNING, WILL REFACTOR LATER

void rpc_send_recv(ser_buff_t* client_send_ser_buffer, ser_buff_t* client_recv_ser_buffer) {
  int* sockfd = socklib_socket_create();
  if (*sockfd == -1) {
    printf("ERROR:: REST - Socket creation failed...\n");
    return;
  }

  struct sockaddr_in* dest = socklib_socket_build_sock_addr_in(sockfd, AF_INET, RPC_SERVER_PORT);
  if (dest == NULL) {
    printf("ERROR:: REST - Failed to assign socket address in rpc_send_recv\n");
    free(sockfd);
    free(dest);
    return;
  }

  int rc = 0;
  int recv_size = 0;
  int addr_len = sizeof(struct sockaddr);
  
  rc = socklib_socket_send_to(sockfd,
                             client_send_ser_buffer->buffer,
                             serlib_get_buffer_data_size(client_send_ser_buffer),
                             0, (struct sockaddr*) dest,
                             sizeof(struct sockaddr));

  printf("%s() : %d bytes sent\n", __FUNCTION__, rc);

  recv_size = socklib_socket_recv_from(sockfd, client_recv_ser_buffer->buffer,
                                       serlib_get_buffer_length(client_recv_ser_buffer),
                                       0, (struct sockaddr*)&dest,
                                       &addr_len);

  printf("%s() : %d bytes recieved\n", __FUNCTION__, recv_size);
}

/*
 *
 *
 *
 */
ser_buff_t* multiply_client_stub_marshal(int a, int b) {
  ser_buff_t* client_send_ser_buffer = NULL;
  serlib_init_buffer_of_size(&client_send_ser_buffer, MAX_RECV_SEND_BUFF_SIZE);

  // serialized header shite
  int SERIALIZED_HDR_SIZE = sizeof(struct ser_header_t); // temporary
  serlib_buffer_skip(client_send_ser_buffer, SERIALIZED_HDR_SIZE);

  ser_header_t* ser_header = (ser_header_t*) malloc(sizeof(struct ser_header_t));
  if (!ser_header) {
    printf("ERROR:: REST - Failed to allocate memory for serialized header\n");
    free(client_send_ser_buffer);
    exit(1);
  }

  ser_header->rpc_proc_id = 55;
  ser_header->payload_size = 0;

  serlib_serialize_data_string(client_send_ser_buffer, (char*)&a, sizeof(int));
  serlib_serialize_data_string(client_send_ser_buffer, (char*)&b, sizeof(int));

  // now that we have payload size
  // resume serialized header shite
  ser_header->payload_size = serlib_get_buffer_data_size(client_send_ser_buffer) - SERIALIZED_HDR_SIZE;

  serlib_copy_in_buffer_by_size(client_send_ser_buffer,
                                sizeof(ser_header->rpc_proc_id),
                                (char*)&ser_header->rpc_proc_id,
                                0); 

  serlib_copy_in_buffer_by_size(client_send_ser_buffer,
                                sizeof(ser_header->payload_size),
                                (char*)&ser_header->payload_size,
                                sizeof(ser_header->rpc_proc_id)); 

  return client_send_ser_buffer;
}

/*
 *
 *
 *
 */
int multiply_client_stub_unmarshal(ser_buff_t* client_recv_ser_buffer) {
  int res = 0;

  serlib_deserialize_data_string((char*)&res, client_recv_ser_buffer, sizeof(int));

  return res;
}

void init_rpc_infra() {
  
}


int multiply_rpc(int a, int b) {
  init_rpc_infra();

  ser_buff_t* client_send_ser_buffer = multiply_client_stub_marshal(a, b);
  ser_buff_t* client_recv_ser_buffer = NULL;

  serlib_init_buffer_of_size(&client_recv_ser_buffer, MAX_RECV_SEND_BUFF_SIZE);

  rpc_send_recv(client_send_ser_buffer, client_recv_ser_buffer);
  
  serlib_free_buffer(client_send_ser_buffer);
  client_send_ser_buffer = NULL;

  int res = multiply_client_stub_unmarshal(client_recv_ser_buffer);

  serlib_free_buffer(client_recv_ser_buffer);

  return res;
}

int main(int argc, char** argv) {
  int a = 20;
  int b = 10;
  int c = 5;

  printf("Calling `multiply_rpc` with a: %d, b: %d\n", a, b);
  int res = multiply_rpc(a, b);
  printf("CLIENT - RPC RESPONSE: res = %d\n", res);
  
  printf("Calling `multiply_rpc` with a: %d, b: %d\n", c, a);
  int res_two = multiply_rpc(c, a);
  printf("CLIENT - RPC RESPONSE: res_two = %d\n", res_two);

  return 0;
}


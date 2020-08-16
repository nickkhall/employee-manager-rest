#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "src/headers/common.h"
#include "src/headers/serialize.h"

// @TODO: THIS IS A POC FOR LEARNING, WILL REFACTOR LATER

void rpc_send_recv(ser_buff_t* client_send_ser_buffer, ser_buff_t* client_recv_send_buffer) {
  struct sockaddr_in dest;
  int sockfd = 0, rc = 0, recv_size = 0;
  int addr_len;

  dest.sin_family = AF_INET;
  dest.sin_port = htons(SERVER_PORT);
  struct hostent* host = (struct hostent*) gethostbyname(SERVER_IP);
  
  dest.sin_addr = *((struct in_addr*) host->h_addr);
  addr_len = sizeof(struct sockaddr);

  sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sockfd == -1) {
    printf("ERROR:: Socket creation failed...\n");
    return;
  }

  rc = sendto(sockfd, client_send_ser_buffer->buffer,
              serlib_serialize_get_buffer_data_size(client_send_ser_buffer),
              0, (struct sockaddr*) &dest,
              sizeof(struct sockaddr));

  printf("%s() : %d bytes sent\n", __FUNCTION__, rc);
}

ser_buff_t* multiply_client_stub_marshal(int a, int b) {
  ser_buff_t* client_send_ser_buffer = NULL;
  serlib_serialize_init_buffer_of_size(&client_send_ser_buffer, MAX_RECV_SEND_BUFF_SIZE);

  serlib_serialize_data_string(client_send_ser_buffer, (char*)&a, sizeof(int));
  serlib_serialize_data_string(client_send_ser_buffer, (char*)&b, sizeof(int));

  return client_send_ser_buffer;
}


int multiply_rpc(int a, int b) {
  ser_buff_t* client_send_ser_buffer = multiply_client_stub_marshal(a, b);
  ser_buff_t* client_recv_ser_buffer = NULL;

  serlib_serialize_init_buffer_of_size(&client_recv_ser_buffer, MAX_RECV_SEND_BUFF_SIZE);

  rpc_send_recv(client_send_ser_buffer, client_recv_ser_buffer);
  
  serlib_serialize_free_buffer(client_send_ser_buffer);
}

int main(int argc, char** argv) {
  int a,b;

  int res = multiply_rpc(a, b);

  printf("res = %d\n", res);

  return 0;
}


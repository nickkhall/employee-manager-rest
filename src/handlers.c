#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#include "headers/handlers.h"
#include "headers/employee.h"
#include "headers/serialize.h"
#include "headers/sockets.h"

#define MULTIPLY_ID 55

/*
 * -------------------------------------------------------------
 *
 * -------------------------------------------------------------
 *
 * -------------------------------------------------------------
 *
 * -------------------------------------------------------------
 */
employee_t* get_employee(char* id) {
  // any rpc init functionality ??
  
  // initialize send serialized buffer's 
  // declare recv serialized buffer (NULL)

  // serialize id data

  // initialize recv buffer
  
  // send and receive data

  // free send buffer

  // reset send buffer (NULL)

  // deserialize response (recv_buffer)

  // free recv_buffer

  // return response (employee_t)
};

void empman_rest_send_recv(ser_buff_t* client_send_ser_buffer, ser_buff_t* client_recv_ser_buffer) {
  int* sockfd = socklib_socket_create();
  if (*sockfd == -1) {
    printf("ERROR:: REST - Socket creation failed...\n");
    return;
  }

  struct sockaddr_in* dest = socklib_socket_build_sock_addr_in(sockfd, AF_INET, RPC_SERVER_PORT);
  if (dest == NULL) {
    printf("ERROR:: REST - Failed to assign socket address in empman_rest_send_recv\n");
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
ser_buff_t* empman_rest_serialize_multiply(int a, int b) {
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

  ser_header->tid = 10;
  ser_header->rpc_proc_id = 55;
  ser_header->msg_type = 3;
  ser_header->payload_size = 0;

  serlib_serialize_data_string(client_send_ser_buffer, (char*)&a, sizeof(int));
  serlib_serialize_data_string(client_send_ser_buffer, (char*)&b, sizeof(int));

  // now that we have payload size
  // resume serialized header shite
  ser_header->payload_size = serlib_get_buffer_data_size(client_send_ser_buffer) - SERIALIZED_HDR_SIZE;

  serlib_copy_in_buffer_by_offset(client_send_ser_buffer,
                                sizeof(ser_header->tid),
                                (char*)&ser_header->tid,
                                0); 

  serlib_copy_in_buffer_by_offset(client_send_ser_buffer,
                                sizeof(ser_header->rpc_proc_id),
                                (char*)&ser_header->rpc_proc_id,
                                sizeof(ser_header->tid)); 

  serlib_copy_in_buffer_by_offset(client_send_ser_buffer,
                                sizeof(ser_header->msg_type),
                                (char*)&ser_header->msg_type,
                                sizeof(ser_header->tid) + sizeof(ser_header->rpc_proc_id)); 

  serlib_copy_in_buffer_by_offset(client_send_ser_buffer,
                                sizeof(ser_header->payload_size),
                                (char*)&ser_header->payload_size,
                                (sizeof(ser_header->msg_type)
                                + sizeof(ser_header->tid)
                                + sizeof(ser_header->rpc_proc_id))); 

  return client_send_ser_buffer;
}

/*
 *
 *
 *
 */
int empman_rest_deserialize_multiply(ser_buff_t* client_recv_ser_buffer) {
  int res = 0;

  serlib_deserialize_data_string((char*)&res, client_recv_ser_buffer, sizeof(int));

  return res;
}

void empman_rest_init() {
  
}


int empman_rest_handlers_multiply(int a, int b) {
  empman_rest_init();

  ser_buff_t* client_send_ser_buffer = empman_rest_serialize_multiply(a, b);
  ser_buff_t* client_recv_ser_buffer = NULL;

  serlib_init_buffer_of_size(&client_recv_ser_buffer, MAX_RECV_SEND_BUFF_SIZE);

  empman_rest_send_recv(client_send_ser_buffer, client_recv_ser_buffer);
  
  serlib_free_buffer(client_send_ser_buffer);
  client_send_ser_buffer = NULL;

  int res = empman_rest_deserialize_multiply(client_recv_ser_buffer);

  serlib_free_buffer(client_recv_ser_buffer);

  return res;
}


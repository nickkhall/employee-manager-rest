#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <libexplain/bind.h>
#include <string.h>
#include <fcntl.h> // nonblocking socket

#include <serialize.h>
#include <sockets.h>

#include "../include/common.h"
#include "../include/handlers.h"
#include "../include/server.h"

int* server_init(void)
{
  // create server socket
  int* server_socket = server_new_socket(REST_SERVER_PORT);
  struct sockaddr_in* server_addr = socklib_socket_build_sock_addr_in(server_socket, AF_INET, REST_SERVER_PORT);

  // create client socket
  int client_socket;
  struct sockaddr_in client_addr;

  int last_socket = *server_socket;

  // bind to REST port
  int binded = bind(*server_socket, (struct sockaddr*) &(*server_addr), sizeof(struct sockaddr));
  if (binded < 0) {
    fprintf(stderr, "%s\n", explain_bind(*server_socket, (const struct sockaddr*) server_addr, sizeof(struct sockaddr)));
    exit(EXIT_FAILURE);
  }
  
  // attempt to start listening on REST port
  int listen_status = listen(*server_socket, 40);
  if (listen_status < 0) {
    printf("ERROR :: Employee Manager REST - Failed to listen on port %d\n", REST_SERVER_PORT);
    exit(EXIT_FAILURE);
  }

  int client_addr_len = sizeof(client_addr);
  client_socket = accept(*server_socket, (struct sockaddr*) &client_addr, (socklen_t*) &client_addr_len);
  if (client_socket == -1) {
    perror("accept"); 
  }

  // change the sockets into non-blocking state
  fcntl(last_socket, F_SETFL, O_NONBLOCK);
  fcntl(client_socket, F_SETFL, O_NONBLOCK);

  // create and initialize send/recv buffers
  ser_buff_t** recv_buffer = (ser_buff_t**) malloc(MAX_RECV_BUFF_SIZE);
  ser_buff_t** send_buffer = (ser_buff_t**) malloc(MAX_RECV_BUFF_SIZE);
  server_init_buffers(recv_buffer, send_buffer);
  
  // reset recv buffer
  serlib_reset_buffer(*recv_buffer);  

  int i, n;
  while(1) {
    for (i = *server_socket; i <= last_socket; i++) {
      printf("Round number %d\n", i);
      if (i == *server_socket) {
        client_addr_len = sizeof(struct sockaddr_in);

        if ((client_socket = accept(*server_socket, (struct sockaddr*) &client_addr, (socklen_t*) &client_addr_len)) == -1) {
          perror("accept");
        }

        printf("REST Server: received connection from %d\n", inet_ntoa(client_addr.sin_addr));

        fcntl(client_socket, F_SETFL, O_NONBLOCK);
        last_socket = client_socket;
      } else {
        n = recv(client_socket, &(*(*recv_buffer)->buffer), serlib_get_buffer_length(*recv_buffer), 0);
        if (n < 1) {
          perror("recv - non blocking \n");
          printf("REST Server ERROR:: NON-BLOCKING - Round %d\nData Received - Bytes: %d\n", i, n);
        } else {
          *((*recv_buffer)->buffer + n) = '\0'; 
          printf("Data from client:\n%s\n\n", (*recv_buffer)->buffer);
          if (send(client_socket, "Hello from the server! :)\n", 26, 0) == -1) {
            perror("send");
          }
        }
      }
    }
  }   
}

int* server_new_socket(int port) {
  // create socket memory
  int* server_socket = (int*) malloc(sizeof(int));
  if (!server_socket) {
    printf("ERROR:: REST - Failed to allocate memory for socket on port %d\n", port);
    return NULL;
  }

  // create tcp socket (2nd param is tcp flag)
  server_socket = socklib_socket_create(REST_SERVER_HOST, port, 1);
  if (*server_socket == -1) {
    printf("ERROR:: REST - Failed to create socket in server_init\n");
    exit(1);
  }

  return server_socket;
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
      printf("default switch case called\n");
      printf("PROCESS ID: %d\n", rest_ser_header->rpc_proc_id);
      break;
  }
}

void server_socket_new_thread(int port,
                              ser_buff_t** recv_buffer,
                              ser_buff_t** send_buffer,
                              struct sockaddr* client_addr,
                              socklen_t* client_addr_len)
{ 
  // create new socket
  int* new_socket = server_new_socket(port);;

  // receive data
  int len = recv(*new_socket, &(*(*recv_buffer)->buffer), serlib_get_buffer_length(*recv_buffer), 0);
  
  // create and apply thread lock
  pthread_mutex_t lock;
  pthread_mutex_init(&lock, NULL);
  pthread_mutex_lock(&lock);

  // print received bytes
  printf("REST server recieved %d bytes\n", len);
  
  // reset send buffer
  serlib_reset_buffer(*send_buffer);
  
  // process request
  server_process_traffic(recv_buffer, send_buffer);

  // unlock the thread's memory
  pthread_mutex_unlock(&lock);
  
  // send the serialized result to client
  len = send(*new_socket, (*send_buffer)->buffer, serlib_get_buffer_length(*send_buffer), 0); 

  // print sent bytes
  printf("Employee Manager REST - Sent %d bytes.\n", len);
  
  // reset send buffer
  serlib_reset_buffer(*send_buffer);

  // close socket
  close(*new_socket);
}


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

#include <serialize.h>
#include <sockets.h>

#include "../include/common.h"
#include "../include/server.h"

int* server_init() {
  // create socket memory
  int* server_socket = (int*) malloc(sizeof(int));
  if (!server_socket) {
    printf("ERROR:: REST - Failed to allocate memory for socket in server_init\n");
    return NULL;
  }

  // create socket
  server_socket = socklib_socket_create(REST_SERVER_PORT);
  if (*server_socket == -1) {
    printf("ERROR:: REST - Failed to create socket in server_init\n");
    exit(1);
  }

  // create server socket
  struct sockaddr_in* server_addr = socklib_socket_build_sock_addr_in(server_socket, AF_INET, REST_SERVER_PORT);

  // bind to rest port
  int binded = bind(*server_socket, (struct sockaddr*) &(*server_addr), sizeof(struct sockaddr));
  if (binded == -1) {
    printf("REST ERROR:: Failed to bind to socket\n");
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
      printf("Default switch\n");
      free(recv_buffer);
      free(send_buffer);
      server_handle_traffic();
      break;
  }
}

void server_socket_new_thread(int* socket,
                              ser_buff_t** recv_buffer,
                              ser_buff_t** send_buffer,
                              struct sockaddr* client_addr,
                              socklen_t* addr_len) { 
  // create new socket
  int new_socket = *socket;

  // receive data
  int len = recv(*socket, &(*(*recv_buffer)->buffer), serlib_get_buffer_length(*recv_buffer), 0);
  
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
  len = send(new_socket, (*send_buffer)->buffer, serlib_get_buffer_length(*send_buffer), 0); 

  // print sent bytes
  printf("Employee Manager REST - Sent %d bytes.\n", len);
  
  // reset send buffer
  serlib_reset_buffer(*send_buffer);

  // close socket
  close(new_socket);
}

void server_handle_traffic()
{
  int* server_new_socket = (int*) malloc(sizeof(int));
  int* server_socket = server_init();
  struct sockaddr_storage server_storage;
  pid_t pid[50];

  // listen and accept up to 40 connections
  // honey badge dont care, honey badger dont giva shit
  int listen_status = listen(*server_socket, 40);

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
  
  if (listen_status == 0) {
    // print running message to screen
    printf("Employee Manager - \nREST - Server is now listening on port %d...\n", REST_SERVER_PORT);
  } else {
    printf("Error\n");
    int i = 0;

    while(1) {
      addr_len = sizeof(server_storage);
      *server_new_socket = accept(*server_socket, (struct sockaddr*) &client_addr, &addr_len);

      int pid_c = 0;

      if ((pid_c = fork()) == 0) {
        server_socket_new_thread(server_new_socket, recv_buffer, send_buffer, (struct sockaddr*) &client_addr, (socklen_t*) &addr_len);
      } else {
        *(pid + (i++)) = pid_c;

        if (i >= 49) {
          i = 0;

          while (i < 50) {
            waitpid(*(pid + (i++)), NULL, 0);
          }
          
          i = 0;
        }
      }
    }
  }    
}


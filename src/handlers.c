#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <sockets.h>
#include <serialize.h>

#include "../include/handlers.h"
#include "../include/employee.h"

/*
 *
 *
 *
 *
 */
employee_t* employees_employee_initialize(employee_t* employee) {
  if (!employee) {
    printf("ERROR:: REST - Failed to allocate memory for employee.\n");
    exit(1);
  }

  employee->id =        (char*) malloc(sizeof(char) * 33);
  employee->first =     (char*) malloc(sizeof(char) * 51);
  employee->last =      (char*) malloc(sizeof(char) * 51);
  employee->email =     (char*) malloc(sizeof(char) * 101);
  employee->address =   (char*) malloc(sizeof(char) * 76);
  employee->phone =     (char*) malloc(sizeof(char) * 51);
  employee->start =     (time_t*) malloc(sizeof(time_t));
  employee->gender =    (char*) malloc(sizeof(char) * 7);
  employee->ethnicity = (char*) malloc(sizeof(char) * 51);
  employee->title =     (char*) malloc(sizeof(char) * 51);
  employee->salary =    (int*) malloc(sizeof(int*));

  return employee;
}

void empman_rest_send_recv(ser_buff_t* client_send_ser_buffer, ser_buff_t* client_recv_ser_buffer) {
  int* sockfd = socklib_socket_create();
  if (*sockfd == -1) {
    printf("ERROR:: REST - Socket creation failed...\n");
    return;
  }

  struct sockaddr_in* dest = socklib_socket_build_sock_addr_in(sockfd, AF_INET, REST_SERVER_PORT);
  if (dest == NULL) {
    printf("ERROR:: REST - Failed to assign socket address in empman_rest_send_recv\n");
    free(sockfd);
    free(dest);
    return;
  }

  int rc = 0;
  int recv_size = 0;
  socklen_t addr_len = sizeof(struct sockaddr);
  
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
 * -------------------------------------------------------------
 * function: empman_rest_handlers_employees_get_id
 * -------------------------------------------------------------
 *
 * -------------------------------------------------------------
 *
 * -------------------------------------------------------------
 */
list_t* empman_rest_handlers_employees_get_id(char* id) {
  // any rpc init functionality ??
  empman_rest_init();

  // initialize send serialized buffer's 
  ser_buff_t* client_send_ser_buffer = empman_rest_serialize_employees_get_id(id);
  // declare recv serialized buffer (NULL)
  ser_buff_t* client_recv_ser_buffer = NULL;

  // initialize recv serialized buffer
  serlib_init_buffer_of_size(&client_recv_ser_buffer, MAX_RECV_SEND_BUFF_SIZE);

  // send and receive data
  empman_rest_send_recv(client_send_ser_buffer, client_recv_ser_buffer);
  
  // free send buffer
  serlib_free_buffer(client_send_ser_buffer);
  // reset send buffer (NULL)
  client_send_ser_buffer = NULL;

  // deserialize response (recv_buffer)
  list_t* employees = empman_rest_deserialize_employees_get_id(client_recv_ser_buffer);
  
  // free recv_buffer
  serlib_free_buffer(client_recv_ser_buffer);

  // return response (employee_t)
  return employees;
};


/*
 *
 *
 *
 *
 */
employee_t* employees_employee_create(char** data) {
  if (!data) {
    printf("ERROR:: REST - Invalid pointer for data in empman_rpc_employees_employee_create\n");
    exit(1);
  }

  employee_t* employee = (employee_t*) malloc(sizeof(employee_t));
  if (!employee) {
    for (int d = 0; d < 11; d++) {
      printf("ERROR:: REST - Failed to allocate memory for employee in empman_rpc_employees_employee_create\n");
      free(*(data + d));
      free(data);
      exit(1);
    }
  }

  employee->id =        (char*) *data;
  employee->first =     (char*) *(data + 1);
  employee->last =      (char*) *(data + 2);
  employee->email =     (char*) *(data + 3);
  employee->address =   (char*) *(data + 4);
  employee->phone =     (char*) *(data + 5);
  employee->start =     (time_t*) *(data + 6);
  employee->ethnicity = (char*) *(data + 7);
  employee->gender =    (char*) *(data + 8);
  employee->title =     (char*) *(data + 9);
  employee->salary =    (int*) (*(data + 10));

  return employee;
}

/*
 * ----------------------------------------------------------------------
 * function: employees_deserialize_employee_t_wrapper
 * ----------------------------------------------------------------------
 * params  : 
 *         > obj - void*
 *         > b   - ser_buff_t*
 * ----------------------------------------------------------------------
 * Generic wrapper function for deserializing an employee.
 * ----------------------------------------------------------------------
 */
void employees_deserialize_employee_t_wrapper(void* data, ser_buff_t* b) {
  employees_deserialize_employee_t(data, b);
}

/*
 * ----------------------------------------------------------------------
 * function: employees_deserialize_employee_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes an employee.
 * ----------------------------------------------------------------------
 */
employee_t* employees_deserialize_employee_t(list_node_t* data, ser_buff_t* b) {
  int sentinel = 0;

  serlib_deserialize_data(b, (char*)&sentinel, sizeof(int));

  if (sentinel == 0xFFFFFFFF) {
    return NULL;
  }

  serlib_buffer_skip(b, (int)(-1 * sizeof(int)));

  employee_t* employee = (employee_t*) malloc(sizeof(employee_t));
  employees_employee_initialize(employee);

  serlib_deserialize_data(b, employee->id,            sizeof(char) * 33);
  serlib_deserialize_data(b, employee->first,         sizeof(char) * 51);
  serlib_deserialize_data(b, employee->last,          sizeof(char) * 51);
  serlib_deserialize_data(b, employee->email,         sizeof(char) * 101);
  serlib_deserialize_data(b, employee->address,       sizeof(char) * 76);
  serlib_deserialize_data(b, employee->phone,         sizeof(char) * 51);
  serlib_deserialize_time_t(b, employee->start, sizeof(time_t*));
  serlib_deserialize_data(b, employee->gender,        sizeof(char) * 7);
  serlib_deserialize_data(b, employee->ethnicity,     sizeof(char) * 51);
  serlib_deserialize_data(b, employee->title,         sizeof(char) * 51);

  serlib_serialize_data(b, (char*)&sentinel, sizeof(int));

  if (sentinel == 0xFFFFFFFF) {
    employee->salary = NULL;
  } else {
    serlib_buffer_skip(b, -1 * sizeof(unsigned long int));
    serlib_deserialize_data_int_ptr(b, employee->salary, sizeof(int*));
  }

  memcpy(data, employee, sizeof(employee_t));
}

/*
 *
 *
 *
 */
ser_buff_t* empman_rest_serialize_employees_get_id(char* id) {
  ser_buff_t* client_send_ser_buffer = NULL;
  serlib_init_buffer_of_size(&client_send_ser_buffer, MAX_RECV_SEND_BUFF_SIZE);

  // serialized header shite
  int SERIALIZED_HDR_SIZE = sizeof(ser_header_t); // temporary
  serlib_buffer_skip(client_send_ser_buffer, SERIALIZED_HDR_SIZE);

  ser_header_t* ser_header = (ser_header_t*) malloc(sizeof(ser_header_t));
  if (!ser_header) {
    printf("ERROR:: REST - Failed to allocate memory for serialized header\n");
    free(client_send_ser_buffer);
    exit(1);
  }

  ser_header->tid = 10;
  ser_header->rpc_proc_id = 55;
  ser_header->rpc_call_id = 3;
  ser_header->payload_size = 0;

  serlib_serialize_data(client_send_ser_buffer, id, sizeof(int));

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
                                sizeof(ser_header->rpc_call_id),
                                (char*)&ser_header->rpc_call_id,
                                sizeof(ser_header->tid) + sizeof(ser_header->rpc_proc_id)); 

  serlib_copy_in_buffer_by_offset(client_send_ser_buffer,
                                sizeof(ser_header->payload_size),
                                (char*)&ser_header->payload_size,
                                (sizeof(ser_header->rpc_call_id)
                                + sizeof(ser_header->tid)
                                + sizeof(ser_header->rpc_proc_id))); 

  return client_send_ser_buffer;
};

list_t* empman_rest_deserialize_employees_get_id(ser_buff_t* client_recv_ser_buffer) {
  ser_header_t* rpc_ser_header = (ser_header_t*) malloc(sizeof(ser_header_t));

  serlib_deserialize_data(client_recv_ser_buffer, (char*)&rpc_ser_header->tid,          sizeof(rpc_ser_header->tid));
  serlib_deserialize_data(client_recv_ser_buffer, (char*)&rpc_ser_header->rpc_proc_id,  sizeof(rpc_ser_header->rpc_proc_id));
  serlib_deserialize_data(client_recv_ser_buffer, (char*)&rpc_ser_header->rpc_call_id,  sizeof(rpc_ser_header->rpc_call_id));
  serlib_deserialize_data(client_recv_ser_buffer, (char*)&rpc_ser_header->payload_size, sizeof(rpc_ser_header->payload_size));

  list_t* employees = serlib_deserialize_list_t(client_recv_ser_buffer, employees_deserialize_employee_t_wrapper);

  return employees;
};


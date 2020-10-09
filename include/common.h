#ifndef __EMP_MAN_REST_COMMON_H__
#define __EMP_MAN_REST_COMMON_H__

#define MAX_RECV_SEND_BUFF_SIZE 2048

#define REST_SERVER_PORT 8745
#define REST_SERVER_HOST "127.0.0.1"

#define RPC_SERVER_PORT 8741
#define RPC_SERVER_HOST "127.0.0.1"

#define MAX_RECV_BUFF_SIZE 2048

typedef enum {
  RPC_REQ,
  RPC_REPLY,
  RPC_AUTH
} rpc_enum_t;

#endif


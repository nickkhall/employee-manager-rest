#ifndef __EMP_MAN_REST_COMMON__
#define __EMP_MAN_REST_COMMON__

#define MAX_RECV_SEND_BUFF_SIZE 2048

#define RPC_SERVER_PORT 8741
#define RPC_SERVER_IP   "127.0.0.1"

typedef enum {
  RPC_REQ,
  RPC_REPLY,
  RPC_AUTH
} rpc_enum_t;

#endif /* __EMP_MAN_REST_COMMON__ */


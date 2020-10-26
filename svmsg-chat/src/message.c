#include "message.h"

void send_message(int id, long type, const char *nickname,
                  const char *message) {
  struct message sendbuf;

  sendbuf.type = type;
  strncpy(sendbuf.nickname, nickname, NAME_LEN);
  strncpy(sendbuf.message, message, MESSAGE_LEN);

  if (msgsnd(id, &sendbuf, MESSAGE_LEN, 0) == -1)
    err("msgsnd", 0);
}


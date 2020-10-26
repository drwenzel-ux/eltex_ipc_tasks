#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <stddef.h>
#include <string.h>
#include <sys/msg.h>

#include "err.h"

typedef enum { connect = 1, disconnect, sendall } message_type;

#define NAME_LEN 16
#define MESSAGE_LEN 32

#define NEW_USER "new"
#define EXIT_USER "del"

struct message {
  long type;
  char nickname[NAME_LEN];
  char message[MESSAGE_LEN];
};

void send_message(int id, long type, const char *nickname, const char *message);

#endif
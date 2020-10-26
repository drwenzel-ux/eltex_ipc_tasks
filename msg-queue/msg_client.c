/* template message msgbuf */
#define _GNU_SOURCE

/* std's */
#include <stdio.h>
#include <stdlib.h>

/* sys */
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

static void err(const char *m, int usage_error) {
  if (usage_error) {
    fprintf(stderr, "usage error: %s\n", m);
    goto usage_error;
  }

  perror(m);

usage_error:
  exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]) {
  int mqid;
  struct msgbuf msg;

  if (argc < 2)
    err("client id", 1);

  mqid = atoi(argv[1]);

  msg.mtype = 1;
  msg.mtext[0] = 'T';

  if (msgsnd(mqid, &msg, 1, 0) == -1)
    err("msgsnd", 0);

  msg.mtype = 4;
  msg.mtext[0] = 'O';

  if (msgsnd(mqid, &msg, 1, 0) == -1)
    err("msgsnd", 0);

  return 0;
}

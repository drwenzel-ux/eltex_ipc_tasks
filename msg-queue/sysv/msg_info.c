#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/msg.h>

static void err_exit(const char *m) {
  perror(m);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  struct msginfo info;
  int s;

  s = msgctl(0, MSG_INFO, (struct msqid_ds *)&info);
  if (s == -1)
    err_exit("msgctl");

  printf("Maximum ID index = %d\n", s);
  printf("queues in_use    = %ld\n", (long)info.msgpool);
  printf("msg_hdrs         = %ld\n", (long)info.msgmap);
  printf("msg_bytes        = %ld\n", (long)info.msgmax);
  exit(EXIT_SUCCESS);
}
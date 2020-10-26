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
  key_t k;
  int id, flags, mqid, len;
  unsigned int perms;
  struct msgbuf msg;
  const char *path;

  if (argc < 3)
    err("ftok path key", 1);

  /* file to key */
  path = argv[1];
  id = atoi(argv[2]);
  if ((k = ftok(path, id)) == -1)
    err("ftok failed", 0);

  printf("key: 0x%08x\n", k);

  /* create mq */
  perms = S_IRUSR | S_IWUSR;
  flags = IPC_CREAT | IPC_EXCL;
  if ((mqid = msgget(k, perms | flags)) == -1)
    err("msgget failed", 0);

  printf("message queue id: %d\n", mqid);

  /* blocking recv */
  if ((len = msgrcv(mqid, &msg, 1, 4, 0)) == -1)
    err("msgrcv", 0);

  printf("received: type=%ld; length=%d; body=%c\n", msg.mtype, len,
         msg.mtext[0]);

  /* rm mq */
  if (msgctl(mqid, IPC_RMID, NULL) == -1)
    err("msgctl", 0);

  exit(EXIT_SUCCESS);
}

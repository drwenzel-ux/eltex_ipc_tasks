#include "utils.h"

void set_handler(void *handler, int signum, int flags) {
  struct sigaction sa;

  sa.sa_handler = handler;
  sa.sa_flags = flags;
  sigemptyset(&sa.sa_mask);

  sigaction(signum, &sa, 0);
}

int mqcreate(const char *path, int id, int perms, int flags) {
  int msqid;
  key_t key;

  if ((key = ftok(path, id)) == -1)
    err("ftok failed", 0);

  if ((msqid = msgget(key, perms | flags)) == -1)
    err("msgget failed", 0);

  return msqid;
}

void mqdestroy(int id) {
  if (msgctl(id, IPC_RMID, NULL) == -1)
    err("msgctl", 0);
}

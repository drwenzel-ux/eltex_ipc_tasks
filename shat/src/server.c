#include "common.h"

volatile sig_atomic_t stop;

void sighandler(int signum) { stop = 1; }

int main(int argc, char const *argv[]) {
  int semid;
  int shmid;
  int retval;

  semid = semget(IPC_PRIVATE, 2, IPC_CREAT | PERMS);
  if (semid == -1)
    err("semget failed", 0);

  sem_unlock(semid, READ, 0);
  sem_unlock(semid, WRITE, 0);

  shmid = shmget(IPC_CREAT, BUFSZ, IPC_CREAT | PERMS);
  if (shmid == -1)
    err("shmget failed", 0);

  set_handler(sighandler, SIGINT, 0);

  printf("semid %d, shmid %d\n", semid, shmid);
  for (;;) {
    if (stop)
      break;
  }

  retval = shmctl(shmid, IPC_RMID, 0);
  if (retval == -1)
    err("shmctl failed", 0);

  retval = semctl(semid, 0, IPC_RMID, 0);
  if (retval == -1)
    err("semctl failed", 0);

  return 0;
}

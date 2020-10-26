#include "common.h"

int main(int argc, char const *argv[]) {
  int semid;
  int shmid;
  struct message *shmp;

  semid = semget(IPC_PRIVATE, 2, IPC_CREAT | PERMS);
  if (semid == -1)
    err("semget failed", 0);

  shmid = shmget(IPC_PRIVATE, sizeof(struct message), IPC_CREAT | PERMS);
  if (shmid == -1)
    err("shmget failed", 0);

  printf("shmid = %d\nsemid = %d\n", shmid, semid);

  shmp = shmat(shmid, NULL, 0);
  if (shmp == (void *)-1)
    err("shmat failed", 0);

  post(semid, WRITE_SEM, 0);

  for (;;) {
    wait(semid, WRITE_SEM, 0);

    shmp->cnt = read(STDIN_FILENO, shmp->buf, SIZE);
    if (shmp->cnt == -1)
      err("read failed", 0);

    post(semid, READ_SEM, 0);

    if (shmp == 0)
      break;
  }

  wait(semid, WRITE_SEM, 0);

  if (semctl(semid, 0, IPC_RMID) == -1)
    err("semctl failed", 0);
  if (shmdt(shmp) == -1)
    err("shmdt failed", 0);
  if (shmctl(shmid, IPC_RMID, 0) == -1)
    err("shmctl failed", 0);

  return 0;
}

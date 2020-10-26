#include "common.h"

int main(int argc, char *argv[]) {
  int semid;
  int shmid;
  struct message *shmp;

  if (argc < 3)
    err("usage", 1);

  semid = atoi(argv[1]);
  shmid = atoi(argv[2]);

  shmp = shmat(shmid, NULL, SHM_RDONLY);
  if (shmp == (void *)-1)
    err("shmat failed", 0);

  for (;;) {
    wait(semid, READ_SEM, 0);

    if (shmp->cnt == 0)
      break;

    if (write(STDOUT_FILENO, shmp->buf, shmp->cnt) != shmp->cnt)
      err("write failed", 0);

    post(semid, WRITE_SEM, 0);
  }

  if (shmdt(shmp) == -1)
    err("shmdt failed", 0);

  post(semid, WRITE_SEM, 0);
  return 0;
}

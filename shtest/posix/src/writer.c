#include "co2mon.h"

int main(int argc, char const *argv[]) {
  int fd;
  int shmid;
  sem_t *writer_sem;
  sem_t *reader_sem;
  struct message *shmp;

  writer_sem = sem_open(WRITER_SEM, O_CREAT | O_RDWR, PERMS, 1);
  reader_sem = sem_open(READER_SEM, O_CREAT | O_RDWR, PERMS, 0);

  if (writer_sem == SEM_FAILED || reader_sem == SEM_FAILED)
    err("sem_open failed", 0);

  fd = shm_open(SHM, O_CREAT | O_RDWR, PERMS);
  if (fd == -1)
    err("shm_open", 0);

  if (ftruncate(fd, SIZE) == -1)
    err("ftruncate", 0);

  shmp = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (shmp == MAP_FAILED)
    err("mmap", 0);

  for (;;) {
    sem_wait(writer_sem);

    shmp->cnt = read(STDIN_FILENO, shmp->buf, SIZE);
    if (shmp->cnt == -1)
      err("read failed", 0);

    sem_post(reader_sem);

    if (shmp->cnt == 0)
      break;
  }

  sem_wait(writer_sem);

  shm_unlink(SHM);
  sem_unlink(READER_SEM);
  sem_unlink(WRITER_SEM);
  return 0;
}

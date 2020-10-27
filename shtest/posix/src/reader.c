#include "co2mon.h"

int main(int argc, char const *argv[]) {
  int fd;
  struct message *shmp;
  struct stat sb;
  sem_t *writer_sem;
  sem_t *reader_sem;

  writer_sem = sem_open(WRITER_SEM, O_RDWR, PERMS);
  reader_sem = sem_open(READER_SEM, O_RDWR, PERMS);

  if (writer_sem == SEM_FAILED || reader_sem == SEM_FAILED)
    err("sem_open failed", 0);

  fd = shm_open(SHM, O_RDONLY, 0);
  if (fd == -1)
    err("shm_open", 0);

  if (fstat(fd, &sb) == -1)
    err("fstat", 0);

  shmp = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
  if (shmp == MAP_FAILED)
    err("mmap", 0);

  for (;;) {
    sem_wait(reader_sem);

    if (shmp->cnt == 0)
      break;

    if (write(STDOUT_FILENO, shmp->buf, shmp->cnt) != shmp->cnt)
      err("write failed", 0);

    sem_post(writer_sem);
  }

  sem_post(writer_sem);
  return 0;
}
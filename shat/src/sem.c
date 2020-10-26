#include "sem.h"

void sem_lock(int id, int type, int flags) {
  struct sembuf lock = {type, -1, flags};

  if (semop(id, &lock, 1) == -1) {
    perror("sem_lock");
    exit(EXIT_FAILURE);
  }
}

void sem_unlock(int id, int type, int flags) {
  struct sembuf unlock = {type, 1, flags};

  if (semop(id, &unlock, 1) == -1) {
    perror("sem_lock");
    exit(EXIT_FAILURE);
  }
}
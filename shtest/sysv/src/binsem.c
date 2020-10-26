#include "binsem.h"

void wait(int id, int nth, int flags) {
  struct sembuf lock = {nth, -1, 0};

  if (semop(id, &lock, 1) == -1) {
    perror("binsem: wait");
    exit(EXIT_FAILURE);
  }
}

void post(int id, int nth, int flags) {
  struct sembuf unlock[2] = {{nth, 0, flags}, {nth, 1, flags}};

  if (semop(id, unlock, 2) == -1) {
    perror("binsem: post");
    exit(EXIT_FAILURE);
  }
}
#include "utils.h"

static void err(const char *m) {
  perror(m);
  exit(1);
}

int main(int argc, char *argv[]) {
  int sig;
  pid_t pid;

  if (argc < 2) {
    fprintf(stderr, "%s pid sig-num\n", argv[0]);
    exit(1);
  }

  pid = atoi(argv[1]);
  sig = atoi(argv[2]);

  if (kill(pid, sig) == -1)
    err("kill");

  return 0;
}
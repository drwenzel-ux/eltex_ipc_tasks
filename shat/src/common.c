#include "common.h"

void err(const char *m, int usage_error) {
  if (usage_error) {
    fprintf(stderr, "usage error: %s\n", m);
    goto usage_error;
  }

  perror(m);

usage_error:
  exit(EXIT_FAILURE);
}

void set_handler(void *handler, int signum, int flags) {
  struct sigaction sa;

  sa.sa_handler = handler;
  sa.sa_flags = flags;
  sigemptyset(&sa.sa_mask);

  sigaction(signum, &sa, 0);
}
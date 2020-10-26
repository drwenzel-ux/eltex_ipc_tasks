#include "err.h"

void err(const char *m, int usage_error) {
  if (usage_error) {
    fprintf(stderr, "usage error: %s\n", m);
    goto usage_error;
  }

  perror(m);

usage_error:
  exit(EXIT_FAILURE);
}
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/msg.h>
#include <sys/types.h>

static void usage_err(const char *format, ...) {
  va_list argList;

  fflush(stdout);
  fprintf(stderr, "Usage: ");
  va_start(argList, format);
  vfprintf(stderr, format, argList);
  va_end(argList);

  fflush(stderr);
  exit(EXIT_FAILURE);
}

static void err_exit(const char *m) {
  perror(m);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  int j;

  if (argc > 1 && strcmp(argv[1], "--help") == 0)
    usage_err("%s [msqid...]\n", argv[0]);

  for (j = 1; j < argc; j++)
    if (msgctl(atoi(argv[j]), IPC_RMID, NULL) == -1)
      err_exit("msgctl");

  exit(EXIT_SUCCESS);
}
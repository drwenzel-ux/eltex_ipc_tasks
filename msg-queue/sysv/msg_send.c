#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/msg.h>
#include <sys/types.h>

#define MAX_MTEXT 1024

struct mbuf {
  long mtype;
  char mtext[MAX_MTEXT];
};

static void cmd_line_err(const char *format, ...) {
  va_list arg_list;

  fflush(stdout);

  fprintf(stderr, "Command-line usage error: ");
  va_start(arg_list, format);
  vfprintf(stderr, format, arg_list);
  va_end(arg_list);

  fflush(stderr);
  exit(EXIT_FAILURE);
}

static void err_exit(const char *m) {
  perror(m);
  exit(EXIT_FAILURE);
}

static void usage_error(const char *progName, const char *msg) {
  if (msg != NULL)
    fprintf(stderr, "%s", msg);
  fprintf(stderr, "Usage: %s [-n] msqid msg-type [msg-text]\n", progName);
  fprintf(stderr, "    -n       Use IPC_NOWAIT flag\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  int msqid, flags, msg_len;
  struct mbuf msg;
  int opt;

  /* Parse command-line options and arguments */

  flags = 0;
  while ((opt = getopt(argc, argv, "n")) != -1) {
    if (opt == 'n')
      flags |= IPC_NOWAIT;
    else
      usage_error(argv[0], NULL);
  }

  if (argc < optind + 2 || argc > optind + 3)
    usage_error(argv[0], "Wrong number of arguments\n");

  msqid = atoi(argv[optind]);
  msg.mtype = atoi(argv[optind + 1]);

  if (argc > optind + 2) {
    msg_len = strlen(argv[optind + 2]) + 1;
    if (msg_len > MAX_MTEXT)
      cmd_line_err("msg-text too long (max: %d characters)\n", MAX_MTEXT);

    memcpy(msg.mtext, argv[optind + 2], msg_len);

  } else {
    msg_len = 0;
  }

  /* Send message */

  if (msgsnd(msqid, &msg, msg_len, flags) == -1)
    err_exit("msgsnd");

  exit(EXIT_SUCCESS);
}
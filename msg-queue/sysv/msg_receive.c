#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/msg.h>
#include <sys/types.h>

#define MAX_MTEXT 1024

struct mbuf {
  long mtype;            /* Message type */
  char mtext[MAX_MTEXT]; /* Message body */
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
  fprintf(stderr, "Usage: %s [options] msqid [max-bytes]\n", progName);
  fprintf(stderr, "Permitted options are:\n");
  fprintf(stderr, "    -e       Use MSG_NOERROR flag\n");
  fprintf(stderr, "    -t type  Select message of given type\n");
  fprintf(stderr, "    -n       Use IPC_NOWAIT flag\n");
#ifdef MSG_EXCEPT
  fprintf(stderr, "    -x       Use MSG_EXCEPT flag\n");
#endif
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  int msqid, flags, type;
  ssize_t msgLen;
  size_t maxBytes;
  struct mbuf msg; /* Message buffer for msgrcv() */
  int opt;         /* Option character from getopt() */

  /* Parse command-line options and arguments */

  flags = 0;
  type = 0;
  while ((opt = getopt(argc, argv, "ent:x")) != -1) {
    switch (opt) {
    case 'e':
      flags |= MSG_NOERROR;
      break;
    case 'n':
      flags |= IPC_NOWAIT;
      break;
    case 't':
      type = atoi(optarg);
      break;
#ifdef MSG_EXCEPT
    case 'x':
      flags |= MSG_EXCEPT;
      break;
#endif
    default:
      usage_error(argv[0], NULL);
    }
  }

  if (argc < optind + 1 || argc > optind + 2)
    usage_error(argv[0], "Wrong number of arguments\n");

  msqid = atoi(argv[optind]);
  maxBytes = (argc > optind + 1) ? atoi(argv[optind + 1]) : MAX_MTEXT;

  /* Get message and display on stdout */

  msgLen = msgrcv(msqid, &msg, maxBytes, type, flags);
  if (msgLen == -1)
    err_exit("msgrcv");

  printf("Received: type=%ld; length=%ld", msg.mtype, (long)msgLen);
  if (msgLen > 0)
    printf("; body=%s", msg.mtext);
  printf("\n");

  exit(EXIT_SUCCESS);
}
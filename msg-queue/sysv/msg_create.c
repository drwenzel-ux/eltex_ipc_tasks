#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

static void cmd_line_err(const char *format, ...) {
  va_list argList;

  fflush(stdout);

  fprintf(stderr, "Command-line usage error: ");
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

static void usage_error(const char *progName, const char *msg) {
  if (msg != NULL)
    fprintf(stderr, "%s", msg);
  fprintf(stderr,
          "Usage: %s [-cx] {-f pathname | -k key | -p} "
          "[octal-perms]\n",
          progName);
  fprintf(stderr, "    -c           Use IPC_CREAT flag\n");
  fprintf(stderr, "    -x           Use IPC_EXCL flag\n");
  fprintf(stderr, "    -f pathname  Generate key using ftok()\n");
  fprintf(stderr, "    -k key       Use 'key' as key\n");
  fprintf(stderr, "    -p           Use IPC_PRIVATE key\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  int count;
  int flags, msqid, opt;
  unsigned int perms;
  long lkey;
  key_t key;

  /* Parse command-line options and arguments */

  count = 0;
  flags = 0;

  while ((opt = getopt(argc, argv, "cf:k:px")) != -1) {
    switch (opt) {
    case 'c':
      flags |= IPC_CREAT;
      break;

    case 'f':
      key = ftok(optarg, 1);
      if (key == -1)
        err_exit("ftok");
      count++;
      break;

    case 'k':
      if (sscanf(optarg, "%li", &lkey) != 1)
        cmd_line_err("-k option requires a numeric argument\n");
      key = lkey;
      count++;
      break;

    case 'p':
      key = IPC_PRIVATE;
      count++;
      break;

    case 'x':
      flags |= IPC_EXCL;
      break;

    default:
      usage_error(argv[0], "Bad option\n");
    }
  }

  if (count != 1)
    usage_error(argv[0], "Exactly one of the options -f, -k, "
                         "or -p must be supplied\n");

  perms =
      (optind == argc) ? (S_IRUSR | S_IWUSR) : strtol(argv[optind], NULL, 8);

  /* Create message queue */

  msqid = msgget(key, flags | perms);
  if (msqid == -1)
    err_exit("msgget");

  printf("%d\n", msqid);
  exit(EXIT_SUCCESS);
}

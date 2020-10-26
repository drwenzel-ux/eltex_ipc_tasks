#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NAME "/posix"

int main(int argc, char const *argv[]) {
  mqd_t md;
  size_t len;
  unsigned pri;
  const char *message;

  if (argc < 3) {
    fprintf(stderr, "usage error\n");
    exit(EXIT_FAILURE);
  }

  len = strlen(argv[1]);
  pri = (unsigned)atoi(argv[2]);
  message = argv[1];

  /* create mq */
  md = mq_open(NAME, O_CREAT | O_RDWR, 0600, NULL);
  if (md == -1) {
    perror("mq_open");
    exit(EXIT_FAILURE);
  }

  /* send */
  if (mq_send(md, message, len, pri) == -1) {
    perror("mq_send");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

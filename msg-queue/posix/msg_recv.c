#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  mqd_t md;
  ssize_t num;
  void *buffer;
  unsigned prio;
  struct mq_attr attr;

  if (argc < 2) {
    fprintf(stderr, "usage error\n");
    exit(EXIT_FAILURE);
  }

  md = mq_open(argv[1], O_RDONLY);
  if (md == -1) {
    perror("mq_open");
    exit(EXIT_FAILURE);
  }

  if (mq_getattr(md, &attr) == -1) {
    perror("mq_getattr");
    exit(EXIT_FAILURE);
  }

  buffer = malloc(attr.mq_msgsize);
  if (buffer == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  num = mq_receive(md, buffer, attr.mq_msgsize, &prio);
  if (num == -1) {
    perror("mq_receive");
    exit(EXIT_FAILURE);
  }

  printf("read %ld bytes; priority = %u\n", (long)num, prio);

  if (write(STDOUT_FILENO, buffer, num) == -1) {
    perror("write");
    exit(EXIT_FAILURE);
  }

  write(STDOUT_FILENO, "\n", 1);

  exit(EXIT_SUCCESS);
}
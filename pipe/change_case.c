#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static void err(const char *m) {
  perror(m);
  exit(1);
}

int main(int argc, char const *argv[]) {
  ssize_t num;
  ssize_t ret;
  char buf[10];
  int fd[2], fd1[2];

  if (pipe(fd) == -1)
    err("first pipe failed");

  if (pipe(fd1) == -1)
    err("second pipe failed");

  switch (fork()) {
  case -1:
    err("fork failed");

  /* child */
  case 0:
    if (close(fd[1]) == -1)
      err("child: first close");

    if (close(fd1[0]) == -1)
      err("child: second close");

    while (1) {
      if ((num = read(fd[0], buf, sizeof(buf))) == -1)
        err("child: read failed");

      if (!num)
        break;

      for (size_t i = 0; i < num - 1; i++)
        buf[i] = (char)(buf[i] - 32);

      if (write(fd1[1], buf, num) == -1)
        err("child: write failed");

      if (!strncmp(buf, "EXIT", 4))
        break;
    }

    if (close(fd[0]) == -1)
      err("child: third close");

    if (close(fd1[1]) == -1)
      err("child: fourth close");

    _exit(EXIT_SUCCESS);

  /* parent  */
  default:
    if (close(fd[0]) == -1)
      err("parent: first close");

    if (close(fd1[1]) == -1)
      err("parent: second close");

    while (1) {
      memset(buf, 0, sizeof(buf));
      if ((num = read(STDIN_FILENO, buf, sizeof(buf))) == -1)
        err("parent: first read failed");

      if (!num)
        break;

      if (write(fd[1], buf, num) == -1)
        err("parent: first write failed");

      if ((num = read(fd1[0], buf, num)) == -1)
        err("parent: first read failed");

      if (write(STDOUT_FILENO, buf, num) == -1)
        err("parent: second write failed");

      if (!strncmp(buf, "EXIT", 4))
        break;
    }

    if (wait(NULL) == -1)
      err("parent: wait");

    if (close(fd[1]) == -1)
      err("parent: third close");

    if (close(fd1[0]) == -1)
      err("parent: fourth close");

    exit(0);
  }
}

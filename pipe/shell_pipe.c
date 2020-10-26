#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void err(const char *m) {
  perror(m);
  exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]) {
  int fd[2];

  if (pipe(fd) == -1)
    err("pipe failed");

  switch (fork()) {
  case -1:
    err("fork failed");

  case 0:
    /* first child */
    if (close(fd[0]) == -1)
      err("child #1: first close");

    if (fd[1] != STDOUT_FILENO) {
      if (dup2(fd[1], STDOUT_FILENO) == -1)
        err("child #1: dup2");

      if (close(fd[1]) == -1)
        err("child #1: second close");
    }

    // execlp("ls", "ls", NULL);
    execlp("netstat", "netstat", "-a", NULL);
    exit(EXIT_SUCCESS);

  default:
    break;
  }

  switch (fork()) {
  case -1:
    err("fork failed");

  case 0:
    /* second child */
    if (close(fd[1]) == -1)
      err("child #2: first close");

    if (fd[0] != STDIN_FILENO) {
      if (dup2(fd[0], STDIN_FILENO) == -1)
        err("child #2: dup2");

      if (close(fd[0]) == -1)
        err("child #2: second close");
    }

    // execlp("wc", "wc", "-l", NULL);
    execlp("grep", "grep", "udp", NULL);
    exit(EXIT_SUCCESS);

  default:
    break;
  }

  if (close(fd[0]) == -1)
    err("parent: first close");

  if (close(fd[1]) == -1)
    err("parent: second close");

  if (wait(NULL) == -1)
    err("parent: first wait");

  if (wait(NULL) == -1)
    err("parent: second wait");

  exit(0);
}

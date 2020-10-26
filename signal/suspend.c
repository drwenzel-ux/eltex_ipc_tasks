#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void handler(int sig) {}

static void err(const char *m) {
  perror(m);
  exit(1);
}

int main(int argc, char *argv[]) {
  int nums, scnt;
  pid_t cpid;
  sigset_t blocked, empty;
  struct sigaction sa;

  if (argc < 2)
    exit(1);

  nums = atoi(argv[1]);

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handler;
  if (sigaction(SIGUSR1, &sa, NULL) == -1)
    err("sigaction");

  sigemptyset(&blocked);
  sigaddset(&blocked, SIGUSR1);
  if (sigprocmask(SIG_SETMASK, &blocked, NULL) == -1)
    err("sigprocmask");

  sigemptyset(&empty);

  switch (cpid = fork()) {
  case -1:
    err("fork");

  case 0: /* child */
    for (scnt = 0; scnt < nums; scnt++) {
      printf("child send sigusr1 %d\n", scnt);
      if (kill(getppid(), SIGUSR1) == -1)
        err("kill");

      printf("child recieve sigusr1 %d\n", scnt);
      if (sigsuspend(&empty) == -1 && errno != EINTR)
        err("sigsuspend");
    }
    exit(0);

  default: /* parent */
    for (scnt = 0; scnt < nums; scnt++) {
      printf("parend receive sigusr1 %d\n", scnt);
      if (sigsuspend(&empty) == -1 && errno != EINTR)
        err("sigsuspend");

      printf("parend send sigusr1 %d\n", scnt);
      if (kill(cpid, SIGUSR1) == -1)
        err("kill");
    }
    exit(0);
  }
}

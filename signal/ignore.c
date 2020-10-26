#include "utils.h"

static void handler(int sig) {
  printf("Caught signal %d (%s)\n", sig, strsignal(sig));
  fflush(NULL);
}

static void err(const char *m) {
  perror(m);
  exit(1);
}

int main(int argc, char const *argv[]) {
  sigset_t pending, blocked;
  const int secs = 5;
  struct sigaction sa;

  printf("setting up handler for sigint\n");
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handler;
  if (sigaction(SIGINT, &sa, 0) == -1)
    err("sigaction");

  sigemptyset(&blocked);
  sigaddset(&blocked, SIGINT);
  if (sigprocmask(SIG_SETMASK, &blocked, 0) == -1)
    err("sigprocmask");

  printf("blocking stdint (%dsec)\n", secs);

  sleep(secs);

  if (sigpending(&pending) == -1)
    err("pending");

  printf("pending sigs:\n");
  print_sigset(stdout, "\t", &pending);

  sleep(2);

  printf("ign sigint\n");
  if (signal(SIGINT, SIG_IGN) == SIG_ERR)
    err("signal");

  if (sigpending(&pending) == -1)
    err("pending");

  if (sigismember(&pending, SIGINT)) {
    printf("sigint is now pending\n");
  } else {
    printf("pending signals are: \n");
    print_sigset(stdout, "\t", &pending);
  }

  sleep(2);

  printf("reestablishing handler for sigint\n");

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handler;
  if (sigaction(SIGINT, &sa, NULL) == -1)
    err("sigaction");

  sleep(2);

  printf("unblocking sigint\n");
  sigemptyset(&blocked);
  if (sigprocmask(SIG_SETMASK, &blocked, NULL) == -1)
    err("sigprocmask");

  return 0;
}

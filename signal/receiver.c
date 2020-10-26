#include "utils.h"

static int sigi[NSIG];
static volatile sig_atomic_t got = 0;

static void handler(int sig) {
  if (sig == SIGINT)
    got = 1;
  else
    sigi[sig]++;
}

static void err(const char *m) {
  perror(m);
  exit(1);
}

int main(int argc, char const *argv[]) {
  int i, secs;
  sigset_t pending, blocking, empty;

  printf("PID is %d\n", getpid());

  struct sigaction sa;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  for (i = 1; i < NSIG; i++) {
    sa.sa_handler = handler;
    sigaction(i, &sa, 0);
  }

  if (argc > 1) {
    secs = atoi(argv[1]);

    sigfillset(&blocking);
    if (sigprocmask(SIG_SETMASK, &blocking, NULL) == -1)
      err("sigprocmask");

    printf("sleeping for %d seconds\n", secs);
    sleep(secs);

    if (sigpending(&pending) == -1)
      err("sigpending");

    printf("pending signals are: \n");
    print_sigset(stdout, "\t", &pending);

    sigemptyset(&empty);
    if (sigprocmask(SIG_SETMASK, &empty, NULL) == -1)
      err("sigprocmask");
  }

  while (!got)
    continue;

  for (i = 1; i < NSIG; i++)
    if (sigi[i] != 0)
      printf("signal %s caught %d time %s\n", strsignal(i), sigi[i],
             (sigi[i] == 1) ? "" : "s");

  return 0;
}

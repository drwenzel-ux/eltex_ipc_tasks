#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  int sig;
  sigset_t mask;
  sigfillset(&mask);
  sigprocmask(SIG_SETMASK, &mask, NULL);

  while (1) {
    sigwait(&mask, &sig);
    printf("Signal: %s\n", strsignal(sig));
    if (sig == SIGINT)
      break;
  }

  return 0;
}

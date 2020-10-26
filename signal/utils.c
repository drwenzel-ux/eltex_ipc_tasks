#include "utils.h"

void print_sigset(FILE *of, const char *prefix, const sigset_t *sigset) {
  int sig, cnt = 0;

  for (sig = 1; sig < NSIG; sig++) {
    if (sigismember(sigset, sig)) {
      fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
      cnt++;
    }
  }

  if (!cnt)
    fprintf(of, "%s empty signal set\n", prefix);
}

int print_sigmask(FILE *of, const char *msg) {
  sigset_t mask;

  if (msg)
    fprintf(of, "%s", msg);

  if (sigprocmask(SIG_BLOCK, NULL, &mask) == -1)
    return -1;

  print_sigset(of, "\t\t", &mask);

  return 0;
}

int print_pendingsigs(FILE *of, const char *msg) {
  sigset_t mask;

  if (msg)
    fprintf(of, "%s", msg);

  if (sigpending(&mask) == -1)
    return -1;

  print_sigset(of, "\t\t", &mask);

  return 0;
}
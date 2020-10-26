#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/msg.h>
#include <sys/types.h>

static void err_exit(const char *m) {
  perror(m);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  int maxind, ind, msqid;
  struct msqid_ds ds;
  struct msginfo msginfo;

  /* Obtain size of kernel 'entries' array */

  maxind = msgctl(0, MSG_INFO, (struct msqid_ds *)&msginfo);
  if (maxind == -1)
    err_exit("msgctl-MSG_INFO");

  printf("maxind: %d\n\n", maxind);
  printf("index     id       key      messages\n");

  /* Retrieve and display information from each element of 'entries' array */

  for (ind = 0; ind <= maxind; ind++) {
    msqid = msgctl(ind, MSG_STAT, &ds);
    if (msqid == -1) {
      if (errno != EINVAL && errno != EACCES)
        err_exit("msgctl-MSG_STAT"); /* Unexpected error */
      continue;                      /* Ignore this item */
    }

    printf("%4d %8d  0x%08lx %7ld\n", ind, msqid,
           (unsigned long)ds.msg_perm.__key, (long)ds.msg_qnum);
  }

  exit(EXIT_SUCCESS);
}
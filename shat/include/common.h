#ifndef __COMMON_H__
#define __COMMON_H__

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/shm.h>
#include <sys/stat.h>

#define NMSZ 32
#define MSGSZ 128
#define BUFSZ 1024

#include "sem.h"

#define PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

void err(const char *m, int usage_error);
void set_handler(void *handler, int signum, int flags);

#endif
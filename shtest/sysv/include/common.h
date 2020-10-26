#ifndef __COMMON_H__
#define __COMMON_H__

#include "binsem.h"
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

#define PERMS 0660

#define SIZE 1024
#define WRITE_SEM 0
#define READ_SEM 1

struct message {
  int cnt;
  char buf[SIZE];
};

void err(const char *m, int usage_error);

#endif
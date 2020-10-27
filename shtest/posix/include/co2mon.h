#ifndef __CO2MON_H__
#define __CO2MON_H__

#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define PERMS 0660

#define SIZE 1024
#define SHM "/message"
#define WRITER_SEM "/writer_sem"
#define READER_SEM "/reader_sem"

struct message {
  int cnt;
  char buf[SIZE];
};

void err(const char *m, int usage_error);

#endif
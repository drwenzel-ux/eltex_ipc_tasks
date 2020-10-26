#ifndef __SEM_H__
#define __SEM_H__

#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdio.h>

#define READ 0
#define WRITE 1

void sem_lock(int id, int type, int flags);
void sem_unlock(int id, int type, int flags);

#endif
#ifndef __BINSEM_H__
#define __BINSEM_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>

void wait(int id, int nth, int flags);
void post(int id, int nth, int flags);

#endif
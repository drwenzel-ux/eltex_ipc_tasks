#ifndef __UTILS_H__
#define __UTILS_H__

#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include "err.h"

void set_handler(void *handler, int signum, int flags);
int mqcreate(const char *path, int id, int perms, int flags);
void mqdestroy(int id);

#endif
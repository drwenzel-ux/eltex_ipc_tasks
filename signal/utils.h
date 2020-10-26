#ifndef __UTILS_H__
#define __UTILS_H__

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_sigset(FILE *of, const char *prefix, const sigset_t *sigset);
int print_sigmask(FILE *of, const char *msg);
int print_pendingsigs(FILE *of, const char *msg);

#endif
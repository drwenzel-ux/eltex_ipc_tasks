#ifndef __TUI_H_
#define __TUI_H_

#include <ncurses.h>
#include <signal.h>
#include <sys/ioctl.h>

#define COLS_INPUT 3

void sig_winch(int signum);
int init_win(WINDOW **window, int dy, int dx, int y, int x);
int init_windows(WINDOW **text, WINDOW **user, WINDOW **message);

void print_list();
void popsym(char *buffer);
void appendsym(char *buffer, int sym);
void print_message(char *buffer, char *name);
void text_print_message(char *buffer, char *name);

#endif
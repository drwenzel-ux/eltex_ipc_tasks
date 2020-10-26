#include "tui.h"
#include "list.h"
#include "message.h"

extern WINDOW *message;
extern WINDOW *text;
extern WINDOW *user;

extern struct list head;

extern int row;
extern int size;

void sig_winch(int signum) {
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  resizeterm(size.ws_row, size.ws_col);
}

int init_win(WINDOW **window, int dy, int dx, int y, int x) {
  WINDOW *winbox;

  winbox = newwin(dy, dx, y, x);
  if (winbox == NULL)
    return -1;

  box(winbox, '|', '-');
  wrefresh(winbox);
  *window = derwin(winbox, dy - 2, dx - 2, 1, 1);
  delwin(winbox);

  return 0;
}

int init_windows(WINDOW **text, WINDOW **user, WINDOW **message) {
  int row, col;
  int max_y, max_x;

  getmaxyx(stdscr, max_y, max_x);

  col = max_x - NAME_LEN - 1;
  row = max_y - COLS_INPUT;

  init_win(text, row, col, 0, 0);
  init_win(user, row, NAME_LEN, 0, col + 1);
  init_win(message, COLS_INPUT, max_x, row, 0);

  return 0;
}

void appendsym(char *buffer, int sym) {
  if (size == MESSAGE_LEN)
    return;

  wmove(message, 0, size);
  wprintw(message, "%c", (char)sym);
  wrefresh(message);

  buffer[size] = (char)sym;
}

void popsym(char *buffer) {
  if (size == 0)
    return;

  wmove(message, 0, size - 1);
  wprintw(message, "%c", ' ');
  wrefresh(message);

  buffer[size] = '\0';
}

void print_message(char *buffer, char *name) {
  text_print_message(buffer, name);

  wmove(message, 0, 0);
  whline(message, ' ', size);
  wrefresh(message);
}

void text_print_message(char *buffer, char *name) {
  wmove(text, row, 0);
  wprintw(text, "%s: %s", name, buffer);
  wrefresh(text);
}

void print_list() {
  wclear(user);
  wmove(user, 0, 0);

  struct list_entry *np;
  LIST_FOREACH(np, &head, entries) {
    wprintw(user, "%.16s\n", (char *)np->data);
  }

  wrefresh(user);
}
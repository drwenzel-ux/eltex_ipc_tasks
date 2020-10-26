#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include "err.h"
#include "list.h"
#include "message.h"
#include "tui.h"
#include "utils.h"

WINDOW *text;
WINDOW *user;
WINDOW *message;

int size;
int row;

int id;
int msqid_read;
int msqid_write;

struct list head;

volatile sig_atomic_t stop;

void handler(int sig) { stop = 1; }

void start_session(const char *name) {
  struct message recvbuf;

  send_message(msqid_read, connect, name, NEW_USER);

  if (msgrcv(msqid_write, &recvbuf, MESSAGE_LEN, 0, MSG_NOERROR) == -1)
    err("msrcv", 0);
  id = atoi(recvbuf.message);

  list_insert(&head, (void *)name, NAME_LEN);
}

void end_session(const char *name) {
  send_message(msqid_read, disconnect, name, EXIT_USER);
  list_remove(&head, (void *)name, NAME_LEN);
}

void *worker(void *arg) {
  struct message recvbuf;

  print_list();
  while (1) {
    if (stop)
      break;

    if (msgrcv(msqid_write, &recvbuf, MESSAGE_LEN, id, MSG_NOERROR) == -1) {
      perror("msgrcv");
      continue;
    }

    if (!strncmp(recvbuf.message, NEW_USER, 3)) {
      list_insert(&head, recvbuf.nickname, NAME_LEN);
      print_list();

    } else if (!strncmp(recvbuf.message, EXIT_USER, 3)) {
      list_remove(&head, recvbuf.nickname, NAME_LEN);
      print_list();

    } else {
      text_print_message(recvbuf.message, recvbuf.nickname);
      row++;
    }
  }

  return NULL;
}

int main(int argc, char const *argv[]) {
  int sym;
  char name[NAME_LEN];
  char buffer[MESSAGE_LEN];

  if (argc < 4)
    err("usage: client read_key write_key name", 1);

  msqid_read = atoi(argv[1]);
  msqid_write = atoi(argv[2]);
  strncpy(name, argv[3], NAME_LEN);

  list_create(&head);

  /* init text ui */
  initscr();
  signal(SIGWINCH, sig_winch);
  cbreak();
  noecho();
  curs_set(FALSE);
  keypad(stdscr, TRUE);
  refresh();
  init_windows(&text, &user, &message);

  /* connect */
  start_session(name);
  set_handler(handler, SIGINT, 0);

  pthread_t thread_id;
  pthread_create(&thread_id, NULL, worker, NULL);

  wmove(text, 0, 0);
  while (1) {
    sym = wgetch(stdscr);

    if (stop)
      break;

    switch (sym) {
    case 10:
      if (size) {
        buffer[size] = '\0';
        send_message(msqid_read, sendall, name, buffer);
        print_message(buffer, name);
        memset(buffer, 0, size);
        size = 0;
        row++;
      }
      break;
    case KEY_BACKSPACE:
      if (size) {
        popsym(buffer);
        size--;
      }
      break;
    default:
      appendsym(buffer, sym);
      size++;
      break;
    }
  }

  pthread_cancel(thread_id);
  
  /* cleanup */
  end_session(name);
  list_removeall(&head);
  endwin();

  return EXIT_SUCCESS;
}
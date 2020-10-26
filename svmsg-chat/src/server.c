#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include "err.h"
#include "hash.h"
#include "list.h"
#include "message.h"
#include "utils.h"

int msqid_read;
int msqid_write;

struct message sendbuf;
struct message recvbuf;

struct list head;
struct hsearch_data tab;

volatile sig_atomic_t stop;

void handler(int sig) { stop = 1; }

void sendall_message(const char *m) {
  int uid;
  struct list_entry *np;

  LIST_FOREACH(np, &head, entries) {
    uid = hash_find_value(&tab, (char *)np->data);

    if (!memcmp(np->data, recvbuf.nickname, NAME_LEN))
      continue;

    if (uid != -1) {
      send_message(msqid_write, uid, recvbuf.nickname, m);
      printf("server: %s -> %.5s [%s]\n", recvbuf.nickname, (char *)np->data,
             m);
    }
  }
}

void connect_case() {
  int new_uid;
  char buffer[10];
  struct list_entry *np;

  /* add new key to hash & gen user identifier */
  hash_add_key(&tab, strdup(recvbuf.nickname));
  new_uid = hash_find_value(&tab, recvbuf.nickname);

  /* send identifier to user */
  sprintf(buffer, "%d", new_uid);
  send_message(msqid_write, recvbuf.type, recvbuf.nickname, buffer);

  /* add nickname to list & send about new users */
  list_insert(&head, recvbuf.nickname, NAME_LEN);
  sendall_message(NEW_USER);

  /* new user sends about itself older users */
  LIST_FOREACH(np, &head, entries) {

    if (!memcmp(np->data, recvbuf.nickname, NAME_LEN))
      continue;

    send_message(msqid_write, new_uid, (char *)np->data, NEW_USER);
    printf("server: %s -> %.5s [%s]\n", (char *)np->data, recvbuf.nickname,
           buffer);
  }
}

void disconnect_case() {
  hash_delete_key(&tab, recvbuf.nickname);
  list_remove(&head, recvbuf.nickname, NAME_LEN);
  sendall_message(EXIT_USER);
}

int main(int argc, char const *argv[]) {
  srand(time(0));

  int id;
  const char *path;

  if (argc < 3)
    err("usage: server path id", 1);

  path = argv[1];
  id = atoi(argv[2]);

  msqid_read = mqcreate(path, id, 0600, IPC_CREAT | IPC_EXCL);
  msqid_write = mqcreate(path, id + 1, 0600, IPC_CREAT | IPC_EXCL);

  printf("server: to connect $client %d %d nick\n", msqid_read, msqid_write);

  hcreate_r(MAX_VALUE, &tab);
  list_create(&head);

  set_handler(handler, SIGINT, 0);
  while (1) {
    if (stop)
      break;

    if (msgrcv(msqid_read, &recvbuf, MESSAGE_LEN, 0, MSG_NOERROR) == -1) {
      perror("msgrcv");
      continue;
    }

    switch (recvbuf.type) {
    case connect:
      printf("server: %s connect\n", recvbuf.nickname);
      connect_case();
      break;

    case disconnect:
      printf("server: %s disconnect\n", recvbuf.nickname);
      disconnect_case();
      break;

    case sendall:
      printf("server: %s sendall %s\n", recvbuf.nickname, recvbuf.message);
      const char *m = recvbuf.message;
      sendall_message(m);
      break;

    default:
      break;
    }
  }

  /* cleanup */
  mqdestroy(msqid_read);
  mqdestroy(msqid_write);

  struct list_entry *np;
  LIST_FOREACH(np, &head, entries) { hash_delete_key(&tab, np->data); }
  hdestroy_r(&tab);
  list_removeall(&head);
  return EXIT_SUCCESS;
}

#include "common.h"
#include <termios.h>

int semid;
int shmid;
int name_len;

char mailbox[MSGSZ];
char buffer[BUFSZ];
char username[NMSZ];

volatile sig_atomic_t stop;

struct termios saved;

void restore(void) { tcsetattr(STDIN_FILENO, TCSANOW, &saved); }

void handler() {
  char *shm_ptr;
  int chat_len;
  int msg_len;

  restore();

  write(STDOUT_FILENO, ">>> ", 6);

  read(STDIN_FILENO, mailbox + name_len, MSGSZ);

  if (!strncmp(mailbox + name_len, "exit", 4)) {
    puts("exit");
    stop = 1;
    return;
  }

  if (!strncmp(mailbox + name_len, "read", 4)) {
    puts("update");
    return;
  }

  sem_lock(semid, READ, 0);
  sem_lock(semid, WRITE, 0);

  /* copy to shared memory */

  shm_ptr = (char *)shmat(shmid, NULL, 0);
  chat_len = strlen(shm_ptr);
  msg_len = strlen(mailbox);
  strncpy(shm_ptr + strlen(shm_ptr), mailbox,
          (msg_len + chat_len) > BUFSZ ? BUFSZ - chat_len : msg_len);
  strncpy(buffer, shm_ptr, BUFSZ);
  shmdt(shm_ptr);

  sem_unlock(semid, WRITE, 0);
  sem_unlock(semid, READ, 0);
}

int main(int argc, char const *argv[]) {

  if (argc < 4)
    err("semid shmid nickname", 1);

  semid = atoi(argv[1]);
  shmid = atoi(argv[2]);
  strncpy(mailbox, argv[3], NMSZ);
  strcat(mailbox, ": ");

  char *shm_ptr;
  name_len = strlen(mailbox);
  set_handler(handler, SIGINT, 0);

  struct termios attributes;
  puts("chat");
  while (1) {

    if (stop)
      break;

    tcgetattr(STDIN_FILENO, &saved);

    tcgetattr(STDIN_FILENO, &attributes);
    attributes.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &attributes);

    sem_lock(semid, READ, 0);

    shm_ptr = (char *)shmat(shmid, NULL, 0);
    strncpy(buffer, shm_ptr, BUFSZ);
    shmdt(shm_ptr);

    sem_unlock(semid, READ, 0);

    write(STDOUT_FILENO, buffer, BUFSZ);

    sleep(1000);
  }

  return 0;
}

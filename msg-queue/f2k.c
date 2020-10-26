#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

typedef int key_t;

static key_t f2k(const char *path, int id) {
  key_t key = 0;
  struct stat sb;

  if (stat(path, &sb) == -1) {
    perror("[f2k] stat failed");
    return -1;
  }

  key |= sb.st_ino & 0xFFFF;
  key |= (sb.st_dev & 0xFF) << 16;
  key |= (id & 0xFF) << 24;

  return key;
}

static void err(const char *m) {
  perror(m);
  exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]) {
  if (argc < 3)
    err("ftok path key");

  key_t k;
  int id = atoi(argv[2]);
  const char *path = argv[1];

  if ((k = f2k(path, id)) == -1)
    err("ftok failed");

  printf("key = 0x%08x\n", k);

  exit(EXIT_SUCCESS);
}

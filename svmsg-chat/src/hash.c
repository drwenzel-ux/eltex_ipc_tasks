#include "hash.h"

int values[MAX_VALUE + 1];

int get_value() {
  int v = 0;

  while (1) {
    v = rand() % MAX_VALUE + 1;
    if (!values[v])
      break;
  }

  values[v] = 1;
  return v;
}

void release_value(int v) { values[v] = 0; }

void hash_add(struct hsearch_data *tab, char *key, int value) {
  ENTRY item = {key, NULL};
  ENTRY *pitem = &item;

  int *id = calloc(1, sizeof(value));
  *id = value;
  item.data = (void *)id;

  if (hsearch_r(item, ENTER, &pitem, tab)) {
    pitem->data = (void *)id;
  }
}

void hash_delete(struct hsearch_data *tab, char *key) {
  ENTRY item = {key};
  ENTRY *pitem = &item;

  if (hsearch_r(item, FIND, &pitem, tab)) {
    free(pitem->data);
    pitem->data = (void *)0;
  }
}

int hash_find_value(struct hsearch_data *tab, char *key) {
  ENTRY item = {key};
  ENTRY *pitem = &item;

  if (hsearch_r(item, FIND, &pitem, tab) && pitem->data != NULL) {
    return *(int *)pitem->data;
  }
  return -1;
}

void hash_add_key(struct hsearch_data *tab, char *key) {
  hash_add(tab, key, get_value());
}

void hash_delete_key(struct hsearch_data *tab, char *key) {
  int v = hash_find_value(tab, key);

  if (v == -1) {
    fprintf(stderr, "hash_delet_key: not found key\n");
    return;
  }

  hash_delete(tab, key);
  release_value(v);
}

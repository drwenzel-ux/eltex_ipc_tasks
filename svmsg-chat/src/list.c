#include "list.h"

void list_create(struct list *head) { LIST_INIT(head); }

void list_insert(struct list *head, void *data, size_t size) {
  struct list_entry *new_node;

  new_node = calloc(1, sizeof(*new_node));
  new_node->data = calloc(1, size);
  memcpy(new_node->data, data, size);

  LIST_INSERT_HEAD(head, new_node, entries);
}

struct list_entry *list_find(struct list *head, void *data, size_t size) {
  struct list_entry *np, *found = NULL;
  LIST_FOREACH(np, head, entries) {
    if (!memcmp(np->data, data, size)) {
      found = np;
      break;
    }
  }
  return found;
}

/* only for string */
void list_print(struct list *head, const char *m) {
  struct list_entry *np;

  LIST_FOREACH(np, head, entries) { printf("%s: %.5s\n", m, (char *)np->data); }
}

void list_remove(struct list *head, void *data, size_t size) {
  struct list_entry *del, *p;

  del = list_find(head, data, size);

  if (del != NULL) {
    p = del;
    LIST_REMOVE(del, entries);
    free(p->data);
    free(p);
  }
}

void list_removeall(struct list *head) {
  struct list_entry *p;

  while (!LIST_EMPTY(head)) {
    p = LIST_FIRST(head);
    LIST_REMOVE(LIST_FIRST(head), entries);
    free(p->data);
    free(p);
  }
}

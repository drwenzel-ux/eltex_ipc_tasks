#ifndef __LIST_H__
#define __LIST_H__

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>

struct list_entry {
  void *data;
  LIST_ENTRY(list_entry) entries;
};

LIST_HEAD(list, list_entry) head;

void list_create(struct list *head);
void list_insert(struct list *head, void *data, size_t s);
struct list_entry *list_find(struct list *head, void *data, size_t size);
void list_remove(struct list *head, void *data, size_t size);
void list_removeall(struct list *head);

/* only for string */
void list_print(struct list *head, const char *m);

#endif
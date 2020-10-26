#ifndef __HASH_H__
#define __HASH_H__

#define _GNU_SOURCE
#define __USE_GNU

#include <search.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VALUE 100

int get_value();
void release_value(int v);
void hash_add(struct hsearch_data *tab, char *key, int value);
void hash_delete(struct hsearch_data *tab, char *key);

void hash_add_key(struct hsearch_data *tab, char *key);
void hash_delete_key(struct hsearch_data *tab, char *key);
int hash_find_value(struct hsearch_data *tab, char *key);

#endif
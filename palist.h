#ifndef PALIST_H
#define PALIST_H

#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>

typedef struct
{
  size_t count, el_size, alloc_count;
  void *data;
} list_t;

list_t *list_new(size_t);
list_t *list_dup(list_t *);
void list_del(list_t *);
void list_clear(list_t *);
void *list_next(list_t *, void *);
void *list_prev(list_t *, void *);
void *list_head(list_t *);
void *list_tail(list_t *);
void *list_at(list_t *, ssize_t);
size_t list_count(list_t *);
bool list_add(list_t *, void *);
void list_remove(list_t *, void *);
void list_remove_tail(list_t *);
void list_for_each(list_t *, void (*)(void *, void *), void *);
void list_reverse(list_t *, list_t *);

#endif

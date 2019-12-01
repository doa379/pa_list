#ifndef PA_LIST_H
#define PA_LIST_H

#include <stdlib.h>

typedef struct
{
  size_t count, el_size, alloc_size;
  void *data;
} list_t;

list_t *list_new(size_t);
void list_del(list_t *);
void list_clear(list_t *);
void *list_next(list_t *, void *);
void *list_prev(list_t *, void *);
void *list_head(list_t *);
void *list_itr_head(list_t *, size_t);
void *list_tail(list_t *);
void *list_itr_tail(list_t *, size_t);
size_t list_count(list_t *);
void list_add(list_t *, void *);
void list_remove(list_t *, void *);
void list_remove_tail(list_t *);

#endif

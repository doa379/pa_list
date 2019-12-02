#ifndef PA_LIST_H
#define PA_LIST_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
  size_t count, el_size, alloc_size;
  void *data;
} pa_list_t;

pa_list_t *pa_list_new(size_t, size_t);
pa_list_t *pa_list_replicate(pa_list_t *);
void pa_list_del(pa_list_t *);
void pa_list_clear(pa_list_t *);
void *pa_list_next(pa_list_t *, void *);
void *pa_list_prev(pa_list_t *, void *);
void *pa_list_head(pa_list_t *);
void *pa_list_itr_head(pa_list_t *, size_t);
void *pa_list_tail(pa_list_t *);
void *pa_list_itr_tail(pa_list_t *, size_t);
size_t pa_list_count(pa_list_t *);
bool pa_list_add(pa_list_t *, void *);
void pa_list_remove(pa_list_t *, void *);
void pa_list_remove_tail(pa_list_t *);
void pa_list_for_each(pa_list_t *, void (*)(char *, void *), void *);

#endif

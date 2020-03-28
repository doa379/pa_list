#include <string.h>
#include "palist.h"

#define ALLOC_INC_COUNT 128

list_t *list_new(size_t el_size)
{
  list_t *list = malloc(sizeof *list);

  if (list == NULL)
    return NULL;

  if ((list->data = calloc(ALLOC_INC_COUNT, el_size)) == NULL)
  {
    free(list);
    return NULL;
  }

  list->count = 0;
  list->el_size = el_size * sizeof(char);
  list->alloc_count = ALLOC_INC_COUNT;
  return list;
}

list_t *list_dup(list_t *list)
{
  list_t *dup = list_new(list->el_size);

  if (dup == NULL)
    return NULL;

  dup->count = list->count;
  
  if ((dup->data = malloc(list->count * list->el_size)) == NULL)
  {
    list_del(dup);
    return NULL;
  }

  memcpy(dup->data, list->data, list->count * list->el_size);
  return dup;
}

void list_del(list_t *list)
{
  if (list->data)
    free(list->data);

  free(list);
}

void *list_head(list_t *list)
{
  if (!list->count)
    return NULL;

  return list->data;
}

void *list_tail(list_t *list)
{
  if (!list->count)
    return NULL;

  return (char *) list->data + (list->count - 1) * list->el_size;
}

bool list_add(list_t *list, void *data)
{
  if (list->count > list->alloc_count - 1)
  {
    void *swap = realloc(list->data, (list->alloc_count + ALLOC_INC_COUNT) * list->el_size);

    if (swap == NULL)
      return 0;

    list->data = swap;
    list->alloc_count += ALLOC_INC_COUNT;
  }

  memcpy((char *) list->data + (list->count * list->el_size), data, list->el_size);
  list->count++;
  return 1;
}

void list_remove(list_t *list, void *data)
{
  if (data == list_tail(list));

  else if (data == list_head(list) && list_count(list) > 1)
    memcpy(data, list_next(list, data), (list_count(list) - 1) * list->el_size);

  else
  {
    size_t i = 0;

    for (void *n = list_tail(list); n; n = list_prev(list, n), i++)
      if (n == data)
      {
        memcpy(n, list_next(list, n), i * list->el_size);
        break;
      }
  }

  list->count--;
}

void *list_next(list_t *list, void *data)
{
  if (data == list_tail(list))
    return NULL;

  return (char *) data + list->el_size;
}

void *list_prev(list_t *list, void *data)
{
  if (data == list_head(list))
    return NULL;

  return (char *) data - list->el_size;
}

void *list_itr_head(list_t *list, size_t n)
{
  if (!list->count)
    return NULL;

  else if (n > list->count - 1)
    return NULL;

  return (char *) list_head(list) + (n * list->el_size);
}

void *list_itr_tail(list_t *list, size_t n)
{
  if (n > list->count - 1)
    return NULL;

  return (char *) list_tail(list) - n * list->el_size;
}

void list_remove_tail(list_t *list)
{
  if (list->count)
    list->count--;
}

size_t list_count(list_t *list)
{
  return list->count;
}

void list_clear(list_t *list)
{
  list->count = 0;
}

void list_for_each(list_t *list, void (*callback)(void *, void *), void *context)
{
  for (void *n = list_head(list); n; n = list_next(list, n))
    callback(n, context);
}

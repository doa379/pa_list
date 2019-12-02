#include <string.h>
#include "pa_list.h"

pa_list_t *pa_list_new(size_t alloc_size, size_t el_size)
{
  pa_list_t *list = malloc(sizeof *list + alloc_size * el_size);
 
  if (list == NULL)
    return NULL;
  
  list->count = 0;
  list->el_size = el_size;
  list->alloc_size = alloc_size;
  return list;
}

void pa_list_del(pa_list_t *list)
{
  free(list);
}

void pa_list_clear(pa_list_t *list)
{
  list->count = 0;
}

void *pa_list_next(pa_list_t *list, void *data)
{
  if (data == pa_list_tail(list))
    return NULL;

  return (char *) data + list->el_size;
}

void *pa_list_prev(pa_list_t *list, void *data)
{
  if (data == pa_list_head(list))
    return NULL;

  return (char *) data - list->el_size;
}

void *pa_list_head(pa_list_t *list)
{
  if (list == NULL || !list->count)
    return NULL;

  return (char *) list + sizeof *list;
}

void *pa_list_itr_head(pa_list_t *list, size_t n)
{
  if (!list->count)
    return NULL;

  else if (n > list->count - 1)
    return NULL;
  
  return (char *) list + sizeof *list + (n * list->el_size);
}

void *pa_list_tail(pa_list_t *list)
{
  if (!list->count)
    return NULL;

  return (char *) list + sizeof *list + (list->count - 1) * list->el_size;
}

void *pa_list_itr_tail(pa_list_t *list, size_t n)
{
  if (n > list->count - 1)
    return NULL;
  
  return (char *) pa_list_tail(list) - n * list->el_size;
}

size_t pa_list_count(pa_list_t *list)
{
  return list->count;
}

bool pa_list_add(pa_list_t *list, void *data)
{
  if (list->count > list->alloc_size - 1)
    {      
      if ((list = realloc(list, sizeof *list + (list->alloc_size + list->alloc_size / 2) * list->el_size)) == NULL)
	return 0;

      list->alloc_size += list->alloc_size / 2;
    }

  memcpy((char *) list + sizeof *list + (list->count * list->el_size), data, list->el_size);
  list->count++;
  return 1;
}

void pa_list_remove(pa_list_t *list, void *data)
{
  size_t i = 0;
  
  for (char *n = pa_list_tail(list); n; n = pa_list_prev(list, n), i++)
    if (n == data)
      {
	memcpy(n, pa_list_next(list, n), i * list->el_size);
	break;
      }

  list->count--;
}

void pa_list_remove_tail(pa_list_t *list)
{
  if (list->count)
    list->count--;
}

void pa_list_for_each(pa_list_t *list, void (*callback)(char *, void *), void *context)
{
  for (char *n = pa_list_head(list); n; n = pa_list_next(list, n))
    callback(n, context);
}

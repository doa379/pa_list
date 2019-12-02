#include <string.h>
#include "pa_list.h"

list_t *list_new(size_t el_size, size_t alloc_size)
{
  list_t *list = malloc(sizeof *list + alloc_size * el_size);
 
  if (list == NULL)
    return NULL;
  
  list->count = 0;
  list->el_size = el_size;
  list->alloc_size = alloc_size;
  return list;
}

void list_del(list_t *list)
{
  free(list);
}

void list_clear(list_t *list)
{
  list->count = 0;
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

void *list_head(list_t *list)
{
  if (list == NULL || !list->count)
    return NULL;

  return (char *) list + sizeof *list;
}

void *list_itr_head(list_t *list, size_t n)
{
  if (!list->count)
    return NULL;

  else if (n > list->count - 1)
    return NULL;
  
  return (char *) list + sizeof *list + (n * list->el_size);
}

void *list_tail(list_t *list)
{
  if (!list->count)
    return NULL;

  return (char *) list + sizeof *list + (list->count - 1) * list->el_size;
}

void *list_itr_tail(list_t *list, size_t n)
{
  if (n > list->count - 1)
    return NULL;
  
  return (char *) list_tail(list) - n * list->el_size;
}

size_t list_count(list_t *list)
{
  return list->count;
}

void list_add(list_t *list, void *data)
{
  if (list->count > list->alloc_size - 1)
    {      
      if ((list = realloc(list, sizeof *list + (list->alloc_size + list->alloc_size / 2) * list->el_size)) == NULL)
	return;

      list->alloc_size += list->alloc_size / 2;
    }

  memcpy((char *) list + sizeof *list + (list->count * list->el_size), data, list->el_size);
  list->count++;
}

void list_remove(list_t *list, void *data)
{
  size_t i = 0;
  
  for (char *n = list_tail(list); n; n = list_prev(list, n), i++)
    if (n == data)
      {
	memcpy((char *) n, (char *) list_next(list, n), i * list->el_size);
	break;
      }

  list->count--;
}

void list_remove_tail(list_t *list)
{
  if (list->count)
    list->count--;
}

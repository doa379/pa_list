#include <string.h>
#include "pa_list.h"

#define LIST_INIT_SIZE 4096
#define LIST_INC_SIZE 256

list_t *list_new(size_t el_size)
{
  list_t *list = malloc(sizeof *list + LIST_INIT_SIZE * el_size);
 
  if (list == NULL)
    return NULL;
  
  list->count = 0;
  list->el_size = el_size;
  list->alloc_size = LIST_INIT_SIZE;
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
  for (size_t i = list->count; i > 0; i--)
    if ((char *) list->data + (i - 1) * list->el_size == data)
      return (char *) list->data + i * list->el_size;

  return NULL;
}

void *list_prev(list_t *list, void *data)
{
  for (size_t i = list->count; i > 0; i--)
    if ((char *) list->data + (i - 1) * list->el_size == data)
      return (char *) list->data + (i - 2) * list->el_size;

  return NULL;
}

void *list_head(list_t *list)
{
  if (list == NULL || !list->count)
    return NULL;

  return (char *) list + sizeof *list;
}

void *list_itr_head(list_t *list, size_t n)
{
  if (list == NULL || !list->count)
    return NULL;

  else if (n > list->count)
    return list_tail(list);
  
  return (char *) list->data + (n * list->el_size);
}

void *list_tail(list_t *list)
{
  if (list == NULL || !list->count)
    return NULL;

  return (char *) list + sizeof *list + (list->count - 1) * list->el_size;
}

void *list_itr_tail(list_t *list, size_t n)
{
  if (n > list->el_size)
    return list_head(list);
  
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
      if ((list->data = realloc(list->data, (list->alloc_size + LIST_INC_SIZE) * list->el_size)) == NULL)
	return;

      list->alloc_size += LIST_INC_SIZE;
    }

  //memcpy((char *) list->data + (list->count * list->el_size), data, list->el_size);
  memcpy((char *) list + sizeof *list + (list->count * list->el_size), data, list->el_size);
  list->count++;
}

void list_remove(list_t *list, void *data)
{
  for (size_t i = list->count; i > 0; i--)
    if ((char *) list->data + (i - 1) * list->el_size == data)
      for (size_t j = i; j < list->count; j++)
	memcpy((char*) list->data + (j - 1) * list->el_size, (char *) list->data + j * list->el_size, list->el_size);

  list->count--;
}

void list_remove_tail(list_t *list)
{
  if (list->count)
    list->count--;
}

/*
   $ cc test_palist.c -L $PWD -l paqueue -Wl,-rpath,$PWD -o test_palist && ./test_palist 
*/

#include <stdio.h>
#include <time.h>
#include "palist.h"

void print(void *n, void *context)
{
  printf("%d(%p) ", **(int **) n, (void *) (int **) n);
}

int main()
{
  list_t *list = list_new(sizeof(int));
  srand(time(NULL));

  for (unsigned i = 0; i < 20; i++)
  {
    int v = rand() % 100;

    if (!list_add(list, &v))
    {
      printf("Realloc failed\n");
      list_del(list);
      return 1;
    }

    printf("%d: List count %zu List Head %d List tail %d\n", v, list_count(list), *(int *) list_head(list), *(int *) list_tail(list));
  }

  printf("\nAccess\n======\n");  
  printf("Iterate from tail\n");
  for (int *n = list_tail(list); n; n = list_prev(list, n))
    printf("%d ", *(int *) n);

  printf("\nIterate from head\n");
  for (int i = 0 ; i < list->count; i++)
    printf("%d ", *(int *) list_at(list, i));

  printf("\nRemove from tail\n");
  for (int *n = list_tail(list), i = 0; n && i < 3; n = list_prev(list, n), i++)
    list_remove(list, n);

  for (int i = 0; i < list->count; i++)
    printf("%d ", *(int *) list_at(list, i));

  printf("\nRemove from head\n");
  for (int *n = list_head(list), i = 0; n && i < 2; n = list_next(list, n), i++)
    list_remove(list, n);

  for (int i = 0; i < list->count; i++)
    printf("%d ", *(int *) list_at(list, i));

  printf("\n");
  list_del(list);

  printf("\nStoring Pointer\n===============\n");
  int V[] = { 4, 7, 8, 9 };
  list_t *list_p = list_new(sizeof(int *));

  for (unsigned i = 0; i < 4; i++)
  {
    int *p = &V[i];
    printf("%p ", (void *) p);
    list_add(list_p, &p);
  }

  list_for_each(list_p, print, NULL);
  printf("\n");

  for (int **i = list_head(list_p); i; i = list_next(list_p, i))
    printf("%d ", **(int **) i);

  printf("\n");
  list_del(list_p);
  return 0;
}

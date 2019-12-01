/*
  $ cc test_pa_list.c -L $PWD -l pa_queue -Wl,-rpath,$PWD -o test_pa_list && ./test_pa_list 
*/

#include <stdio.h>
#include <time.h>
#include "pa_list.h"

int main()
{
  list_t *list = list_new(sizeof(int));
  srand(time(NULL));
  
  for (unsigned i = 0; i < 10; i++)
    {
      int v = rand() % 100;
      list_add(list, &v);
      printf("%d\n", v);
      printf("List Head %d List tail %d\n", *(int *) list_head(list), *(int *) list_tail(list));
    }

  printf("Count %d head %d tail %d\n", list_count(list), *(int *) list_head(list), *(int *) list_tail(list));

  printf("Iterate\n");
  for (int *n = list_tail(list); n; n = list_prev(list, n))
    printf("%d ", *(int *) n);

  printf("\nIterate from head\n");
  for (int i = 0; i < list->count; i++)
    printf("%d ", *(int *) list_itr_head(list, i));

  printf("\nRemove from tail\n");
  for (int *n = list_tail(list), i = 0; n && i < 3; n = list_prev(list, n), i++)
    list_remove(list, n);
  
  for (int i = 0; i < list->count; i++)
    printf("%d ", *(int *) list_itr_head(list, i));

  printf("\nRemove from head\n");
  for (int *n = list_head(list), i = 0; n && i < 2; n = list_next(list, n), i++)
    list_remove(list, n);
  
  for (int i = 0; i < list->count; i++)
    printf("%d ", *(int *) list_itr_head(list, i));
 
  list_del(list);
  return 0;
}

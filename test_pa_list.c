/*
  $ cc test_pa_list.c -L $PWD -l pa_queue -Wl,-rpath,$PWD -o test_pa_list && ./test_pa_list 
*/

#include <stdio.h>
#include <time.h>
#include "pa_list.h"

void print(void *n, void *context)
{
  printf("%d(%p) ", **(int **) n, (int **) n);
}

int main()
{
  pa_list_t *list = pa_list_new(5, sizeof(int));
  srand(time(NULL));
  
  for (unsigned i = 0; i < 7; i++)
    {
      int v = rand() % 100;
      
      if (!pa_list_add(list, &v))
	{
	  printf("Realloc failed\n");
	  pa_list_del(list);
	  exit(0);
	}

      printf("%d: List count %d List Head %d List tail %d\n", v, pa_list_count(list), *(int *) pa_list_head(list), *(int *) pa_list_tail(list));
    }

  printf("\nAccess\n======\n");  
  printf("Iterate from tail\n");
  for (int *n = pa_list_tail(list); n; n = pa_list_prev(list, n))
    printf("%d ", *(int *) n);

  printf("\nIterate from head\n");
  for (int i = 0; i < list->count; i++)
    printf("%d ", *(int *) pa_list_itr_head(list, i));

  printf("\nRemove from tail\n");
  for (int *n = pa_list_tail(list), i = 0; n && i < 3; n = pa_list_prev(list, n), i++)
    pa_list_remove(list, n);
  
  for (int i = 0; i < list->count; i++)
    printf("%d ", *(int *) pa_list_itr_head(list, i));

  printf("\nRemove from head\n");
  for (int *n = pa_list_head(list), i = 0; n && i < 2; n = pa_list_next(list, n), i++)
    //if (i == 1)
      pa_list_remove(list, n);
  
  for (int i = 0; i < list->count; i++)
    printf("%d ", *(int *) pa_list_itr_head(list, i));

  printf("\n");
  pa_list_del(list);

  printf("\nStoring Pointer\n===============\n");
  int V[] = { 4, 7, 8, 9 };
  pa_list_t *list_p = pa_list_new(5, sizeof(int *));

  for (unsigned i = 0; i < 4; i++)
    {
      int *p = &V[i];
      printf("%p ", *p);
      pa_list_add(list_p, &p);
    }

  pa_list_for_each(list_p, print, NULL);
  printf("\n");

  for (int **i = pa_list_head(list_p); i; i = pa_list_next(list_p, i))
    printf("%d ", **(int **) i);
  
  printf("\n");
  pa_list_del(list_p);
  return 0;
}

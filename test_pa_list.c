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

  list_del(list);
  return 0;
}

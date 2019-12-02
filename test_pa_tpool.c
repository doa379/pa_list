/*
  $ cc test_pa_tpool.c -L $PWD -l pa_queue -Wl,-rpath,$PWD -o test_pa_tpool && ./test_pa_tpool
*/

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "pa_tpool.h"

void func_cb(void *context, void *data)
{
  int *v = (int *) data;
  printf("%d\n", *v);
  sleep(5);
  printf("Job exit\n");
}

int main()
{
  pa_tpool_t *pa_tpool = pa_tpool_new(5, NULL);
  srand(time(NULL));

  for (unsigned i = 0; i < 5; i++)
    {
      int v = rand() % 100;
      pa_tpool_queue(pa_tpool, func_cb, &v, sizeof v);
      printf("Data %d job added\n", v);
    }

  while (pa_list_count(pa_tpool->list_jobs_q))
    {
      printf("%d jobs remaining\n", pa_list_count(pa_tpool->list_jobs_q));
      sleep(1);
    }

  pa_tpool_del(pa_tpool);
  return 0;
}

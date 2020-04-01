/*
   $ cc test_patpool.c -L $PWD -l paqueue -Wl,-rpath,$PWD -o test_patpool && ./test_patpool
*/

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "patpool.h"

void func_cb(list_t *ARGS)
{
  arg_t *arg0 = list_at(ARGS, 0);
  int v = *(int *) arg0->arg;
  printf("Job %d start\n", v);
  sleep(1);
  printf("Job %d complete\n", v);
}

int main()
{
  tpool_t *tpool = tpool_new();
  srand(time(NULL));

  for (unsigned i = 0; i < 5; i++)
  {
    int v = rand() % 100;
    tpool_queue(tpool, func_cb, 1, (arg_t) { &v, sizeof v });
    printf("Data %d job added\n", v);
  }

  while (tpool_count(tpool))
  {
    printf("%zu jobs remaining\n", tpool_count(tpool));
    sleep(1);
  }

  tpool_del(tpool);
  return 0;
}

/*
   $ cc test_patpool.c -L $PWD -l paqueue -Wl,-rpath,$PWD -o test_patpool && ./test_patpool
   */

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "patpool.h"

void func_cb(void *data)
{
  int *v = (int *) data;
  printf("Job %d\n", *v);
  sleep(1);
  printf("Job(s) complete, exit\n");
}

int main()
{
  tpool_t *tpool = tpool_new();
  srand(time(NULL));

  for (unsigned i = 0; i < 5; i++)
  {
    int v = rand() % 100;
    tpool_queue(tpool, func_cb, &v, sizeof v);
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

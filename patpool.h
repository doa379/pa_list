#ifndef PATPOOL_H
#define PATPOOL_H

#include <pthread.h>
#include "palist.h"

typedef struct
{
  void (*func)(void *);
  void *arg;
  size_t arg_size;
} job_t;

typedef struct
{
  list_t *jobs_q;
  pthread_mutex_t mutex;
  pthread_cond_t cond_var;
  pthread_t pth;
  bool quit;
} tpool_t;

tpool_t *tpool_new(size_t);
void tpool_del(tpool_t *);
void tpool_clear(tpool_t *);
size_t tpool_count(tpool_t *);
void tpool_queue(tpool_t *, void (*)(void *), void *, size_t);

#endif

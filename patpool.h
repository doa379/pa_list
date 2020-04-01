#ifndef PATPOOL_H
#define PATPOOL_H

#include <pthread.h>
#include <stdarg.h>
#include "palist.h"

typedef struct
{
  void *arg;
  size_t size;
} arg_t;

typedef struct
{
  void (*func)(list_t *);
  list_t *ARGS;
} job_t;

typedef struct
{
  list_t *Q;
  pthread_mutex_t mutex;
  pthread_cond_t cond_var;
  pthread_t pth;
  bool quit;
} tpool_t;

tpool_t *tpool_new(void);
void tpool_del(tpool_t *);
void tpool_clear(tpool_t *);
size_t tpool_count(tpool_t *);
void tpool_queue(tpool_t *, void (*)(list_t *), unsigned, ...);

#endif

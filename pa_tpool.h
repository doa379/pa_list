#ifndef PA_TPOOL_H
#define PA_TPOOL_H

#include <pthread.h>
#include "pa_list.h"

typedef struct
{
  void (*func)(void *, void *);
  void *arg;
  size_t arg_size;
} pa_job_t;

typedef struct
{
  pa_list_t *list_jobs_q;
  void *context;
  pthread_mutex_t mutex;
  pthread_cond_t cond_var;
  pthread_t pth;
  bool quit;
} pa_tpool_t;

pa_tpool_t *pa_tpool_new(size_t, void *);
void pa_tpool_del(pa_tpool_t *);
void pa_tpool_clear(pa_tpool_t *);
void pa_tpool_queue(pa_tpool_t *, void (*)(void *, void *), void *, size_t);

#endif

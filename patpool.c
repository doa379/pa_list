#include <string.h>
#include "patpool.h"

static void arg_del(void *node, void *context)
{
  arg_t *arg = (arg_t *) node;
  
  if (arg->size)
    free(arg->arg);
}

static void job_del(void *node, void *context)
{
  job_t *job = (job_t *) node;
  list_for_each(job->ARGS, arg_del, NULL);
  list_del(job->ARGS);
  list_t *Q = (list_t *) context;
  list_remove(Q, job);
}

size_t tpool_count(tpool_t *tpool)
{
  pthread_mutex_lock(&tpool->mutex);
  size_t count = tpool->Q->count;
  pthread_mutex_unlock(&tpool->mutex);
  return count;
}

void tpool_clear(tpool_t *tpool)
{
  pthread_mutex_lock(&tpool->mutex);
  list_for_each(tpool->Q, job_del, tpool->Q);
  pthread_mutex_unlock(&tpool->mutex);
}

static void queue_pop(tpool_t *tpool)
{
  void *head = list_head(tpool->Q);
  job_del(head, tpool->Q);
}

void tpool_queue(tpool_t *tpool, void (*func)(list_t *), unsigned n, ...)
{
  job_t job = { .func = func, .ARGS = list_new(sizeof(arg_t)) };
  va_list valist;
  va_start(valist, n);

  for (unsigned i = 0; i < n; i++)
  {
    arg_t arg = va_arg(valist, arg_t);

    if (arg.size)
    {
      void *data = malloc(arg.size);
      memcpy(data, arg.arg, arg.size);
      arg.arg = data;
    }
    
    list_add(job.ARGS, &arg);
  }

  va_end(valist);
  pthread_mutex_lock(&tpool->mutex);
  list_add(tpool->Q, &job);
  pthread_mutex_unlock(&tpool->mutex);
  pthread_cond_signal(&tpool->cond_var);
}

static void *worker_th(void *userp)
{
  tpool_t *tpool = userp;

  while (1)
  {
    pthread_mutex_lock(&tpool->mutex);

    while (!list_count(tpool->Q) && !tpool->quit)
      pthread_cond_wait(&tpool->cond_var, &tpool->mutex);

    if (tpool->quit)
    {
      pthread_mutex_unlock(&tpool->mutex);
      return NULL;
    }

    job_t *job = list_head(tpool->Q);
    pthread_mutex_unlock(&tpool->mutex);
    (job->func)(job->ARGS);
    pthread_mutex_lock(&tpool->mutex);
    queue_pop(tpool);
    pthread_mutex_unlock(&tpool->mutex);
  }

  return NULL;
}

void tpool_del(tpool_t *tpool)
{
  tpool->quit = 1;
  tpool_clear(tpool);
  pthread_cond_signal(&tpool->cond_var);
  pthread_join(tpool->pth, NULL);
  pthread_cond_destroy(&tpool->cond_var);
  pthread_mutex_destroy(&tpool->mutex);
  list_del(tpool->Q);
  free(tpool);
}

tpool_t *tpool_new(void)
{
  tpool_t *tpool = calloc(1, sizeof *tpool);
  tpool->Q = list_new(sizeof(job_t));
  pthread_mutex_init(&tpool->mutex, NULL);
  pthread_cond_init(&tpool->cond_var, NULL);
  pthread_create(&tpool->pth, NULL, worker_th, (void *) tpool);
  return tpool;
}

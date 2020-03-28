#include <string.h>
#include "patpool.h"

static void job_del(void *node, void *context)
{
  job_t *job = (job_t *) node;

  if (job->arg_size)
    free(job->arg);

  list_t *list = (list_t *) context;
  list_remove(list, node);
}

void tpool_clear(tpool_t *tpool)
{
  pthread_mutex_lock(&tpool->mutex);
  list_for_each(tpool->jobs_q, job_del, tpool->jobs_q);
  pthread_mutex_unlock(&tpool->mutex);
}

size_t tpool_count(tpool_t *tpool)
{
  pthread_mutex_lock(&tpool->mutex);
  size_t count = tpool->jobs_q->count;
  pthread_mutex_unlock(&tpool->mutex);
  return count;
}

static void queue_pop(tpool_t *tpool)
{
  void *head = list_head(tpool->jobs_q);
  job_del(head, tpool->jobs_q);
}

void tpool_queue(tpool_t *tpool, void (*func)(void *), void *arg, size_t size)
{
  job_t job = { .func = func, .arg = arg, .arg_size = size };

  if (size)
  {
    job.arg = malloc(size);
    memcpy(job.arg, arg, size);
  }

  pthread_mutex_lock(&tpool->mutex);
  list_add(tpool->jobs_q, &job);
  pthread_mutex_unlock(&tpool->mutex);
  pthread_cond_signal(&tpool->cond_var);
}

static void *worker_th(void *userp)
{
  tpool_t *tpool = userp;

  while (1)
  {
    pthread_mutex_lock(&tpool->mutex);

    while (!list_count(tpool->jobs_q) && !tpool->quit)
      pthread_cond_wait(&tpool->cond_var, &tpool->mutex);

    if (tpool->quit)
    {
      pthread_mutex_unlock(&tpool->mutex);
      return NULL;
    }

    job_t *job = list_head(tpool->jobs_q);
    pthread_mutex_unlock(&tpool->mutex);
    (job->func)(job->arg);
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
  list_del(tpool->jobs_q);
  free(tpool);
  tpool = NULL;
}

tpool_t *tpool_new(size_t alloc_size)
{
  tpool_t *tpool = malloc(sizeof *tpool);
  tpool->quit = 0;
  tpool->jobs_q = list_new(alloc_size, sizeof(job_t));
  pthread_mutex_init(&tpool->mutex, NULL);
  pthread_cond_init(&tpool->cond_var, NULL);
  pthread_create(&tpool->pth, NULL, worker_th, (void *) tpool);
  return tpool;
}

#include <string.h>
#include "pa_tpool.h"

static void job_del(char *node, void *context)
{
  job_t *job = (job_t *) node;

  if (job->arg_size)
    free(job->arg);

  pa_list_t *list = (pa_list_t *) context;
  pa_list_remove(list, node);
}

void pa_tpool_clear(pa_tpool_t *pa_tpool)
{
  pthread_mutex_lock(&pa_tpool->mutex);
  pa_list_for_each(pa_tpool->list_jobs_q, job_del, pa_tpool->list_jobs_q);
  pthread_mutex_unlock(&pa_tpool->mutex);
}

static void queue_pop(pa_tpool_t *pa_tpool)
{
  void *head = pa_list_head(pa_tpool->list_jobs_q);
  job_del(head, pa_tpool->list_jobs_q);
}

void pa_tpool_queue(pa_tpool_t *pa_tpool, void (*func)(void *, void *), void *arg, size_t size)
{
  job_t job = {
    .func = func,
    .arg = arg,
    .arg_size = size
  };

  if (size)
    {
      job.arg = malloc(size);
      memcpy(job.arg, arg, size);
    }
  
  pthread_mutex_lock(&pa_tpool->mutex);
  pa_list_add(pa_tpool->list_jobs_q, &job);
  pthread_mutex_unlock(&pa_tpool->mutex);
  pthread_cond_signal(&pa_tpool->cond_var);
}

static void *worker_th(void *userp)
{
  pa_tpool_t *pa_tpool = userp;
  
  while (1)
    {
      pthread_mutex_lock(&pa_tpool->mutex);

      while (!pa_list_count(pa_tpool->list_jobs_q) && !pa_tpool->quit)
	pthread_cond_wait(&pa_tpool->cond_var, &pa_tpool->mutex);

      if (pa_tpool->quit)
	{
	  pthread_mutex_unlock(&pa_tpool->mutex);
	  return NULL;
	}
      
      job_t *job = pa_list_head(pa_tpool->list_jobs_q);
      pthread_mutex_unlock(&pa_tpool->mutex);
      void (*func)(void *, void *) = job->func;
      func(pa_tpool->context, job->arg);
      pthread_mutex_lock(&pa_tpool->mutex);
      queue_pop(pa_tpool);
      pthread_mutex_unlock(&pa_tpool->mutex);
    }

  return NULL;
}

void pa_tpool_del(pa_tpool_t *pa_tpool)
{
  pa_tpool->quit = 1;
  pa_tpool_clear(pa_tpool);
  pthread_cond_signal(&pa_tpool->cond_var);
  pthread_join(pa_tpool->pth, NULL);
  pthread_cond_destroy(&pa_tpool->cond_var);
  pthread_mutex_destroy(&pa_tpool->mutex);
  pa_list_del(pa_tpool->list_jobs_q);
  free(pa_tpool);
  pa_tpool = NULL;
}

pa_tpool_t *pa_tpool_new(size_t alloc_size, void *context)
{
  pa_tpool_t *pa_tpool = malloc(sizeof *pa_tpool);
  pa_tpool->quit = 0;
  pa_tpool->list_jobs_q = pa_list_new(alloc_size, sizeof(job_t));
  pa_tpool->context = context;
  pthread_mutex_init(&pa_tpool->mutex, NULL);
  pthread_cond_init(&pa_tpool->cond_var, NULL);
  pthread_create(&pa_tpool->pth, NULL, worker_th, (void *) pa_tpool);
  return pa_tpool;
}

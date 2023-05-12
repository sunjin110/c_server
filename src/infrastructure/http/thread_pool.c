#include "thread_pool.h"

#include <stdlib.h>

extern void thread_pool_init(thread_pool_t *pool, int thread_count) {
  pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
  pool->thread_count = thread_count;
  pool->queue = (thread_pool_task_queue_t *)malloc(sizeof(thread_pool_task_queue_t));
  thread_pool_task_queue_init(pool->queue, thread_count);

  for (int i = 0; i < thread_count; i++) {
    pthread_create(&pool->threads[i], NULL, thread_pool_worker, pool->queue);
  }
}

extern void thread_pool_task_queue_init(thread_pool_task_queue_t *queue, int size) {
  queue->tasks = (task_t *)malloc(sizeof(task_t) * size);
  queue->front = 0;
  queue->rear = -1;
  queue->count = 0;
  queue->size = size;
  pthread_mutex_init(&queue->lock, NULL);
  pthread_cond_init(&queue->not_empty, NULL);
  pthread_cond_init(&queue->not_full, NULL);
}

extern void thread_pool_submit(thread_pool_t *pool, void (*function)(void *), void *argument) {
  task_t task = {
      .function = function,
      .argument = argument,
  };

  pthread_mutex_lock(&pool->queue->lock);

  // threadが空くまで待つ
  while (pool->queue->count == pool->queue->size) {
    pthread_cond_wait(&pool->queue->not_full, &pool->queue->lock);
  }

  thread_pool_task_queue_push(pool->queue, task);

  pthread_cond_signal(&pool->queue->not_empty);
  pthread_mutex_unlock(&pool->queue->lock);
}

extern void thread_pool_task_queue_push(thread_pool_task_queue_t *queue, task_t task) {
  queue->rear = (queue->rear + 1) % queue->size;
  queue->tasks[queue->rear] = task;
  queue->count++;
}

extern task_t thread_pool_task_queue_pop(thread_pool_task_queue_t *queue) {
  task_t task = queue->tasks[queue->front];
  queue->front = (queue->front + 1) % queue->size;
  queue->count--;
  return task;
}

extern void *thread_pool_worker(void *arg) {
  thread_pool_task_queue_t *queue = (thread_pool_task_queue_t *)arg;

  for (;;) {
    pthread_mutex_lock(&queue->lock);
    while (queue->count == 0) {
      pthread_cond_wait(&queue->not_empty, &queue->lock);
    }

    task_t task = thread_pool_task_queue_pop(queue);

    pthread_cond_signal(&queue->not_full);
    pthread_mutex_unlock(&queue->lock);
    (*(task.function))(task.argument);
  }
}

extern void thread_pool_wait(thread_pool_t *pool) {
  for (int i = 0; i < pool->thread_count; i++) {
    pthread_join(pool->threads[i], NULL);
  }
}

extern void thread_pool_destroy(thread_pool_t *pool) {
  thread_pool_task_queue_destroy(pool->queue);
  free(pool->queue);
  free(pool->threads);
}

extern void thread_pool_task_queue_destroy(thread_pool_task_queue_t *queue) {
  pthread_mutex_destroy(&queue->lock);
  pthread_cond_destroy(&queue->not_empty);
  pthread_cond_destroy(&queue->not_full);
  free(queue->tasks);
  // TODO free(queue) ???
}

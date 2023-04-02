#include <pthread.h>

typedef struct {
  void (*function)(void *);
  void *argument;
} task_t;

typedef struct {
  task_t *tasks;
  int front;
  int rear;
  int count;
  int size;
  pthread_mutex_t lock;
  pthread_cond_t not_empty;
  pthread_cond_t not_full;
} thread_pool_task_queue_t;

typedef struct {
  pthread_t *threads;
  int thread_count;
  thread_pool_task_queue_t *queue;
} thread_pool_t;

void thread_pool_init(thread_pool_t *pool, int thread_count);
void thread_pool_submit(thread_pool_t *pool, void (*function)(void *),
                        void *argument);
void thread_pool_wait(thread_pool_t *pool);
void thread_pool_destroy(thread_pool_t *pool);
void thread_pool_task_queue_init(thread_pool_task_queue_t *queue, int size);
void thread_pool_task_queue_destroy(thread_pool_task_queue_t *queue);
void thread_pool_task_queue_push(thread_pool_task_queue_t *queue, task_t task);
task_t thread_pool_task_queue_pop(thread_pool_task_queue_t *queue);
void *thread_pool_worker(void *arg);

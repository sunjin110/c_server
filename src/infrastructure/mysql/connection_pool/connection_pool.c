#include "connection_pool.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

extern mysql_connection_pool *new_mysql_connection_pool(mysql_config *config,
                                                        int connection_count) {
  mysql_connection_pool *pool =
      (mysql_connection_pool *)malloc(sizeof(mysql_connection_pool));
  pool->max_size = connection_count;
  pool->current_size = 0;
  pthread_mutex_init(&pool->lock, NULL);
  pthread_cond_init(&pool->exists_available_connection, NULL);
  for (int i = 0; i < connection_count; i++) {
    MYSQL *mysql_client = mysql_setup(config);
    if (mysql_client == NULL) {
      printf("======== failed setup mysql\n");
      continue;
    }
    append_mysql_connection(pool, mysql_client);
  }

  return pool;
}

extern void mysql_connection_pool_destroy(mysql_connection_pool *pool) {
  if (pool == NULL) {
    return;
  }

  pool->current_size = 0;
  pthread_mutex_destroy(&pool->lock);
  pthread_cond_destroy(&pool->exists_available_connection);

  mysql_connection_element *current_element = pool->last_ptr;
  for (;;) {
    if (current_element == NULL) {
      break;
    }

    mysql_connection_element *tmp_ptr = current_element->before_ptr;
    mysql_close(current_element->mysql_client);
    current_element->mysql_client = NULL;
    free(current_element);
    current_element = tmp_ptr;
  }

  free(pool);
}

// TODO
// 既にlistに入っているpointerのmysql_clientが入った場合に動作がおかしくなるので
// 将来的にそれを解決する方法を探す必要がある
extern void append_mysql_connection(mysql_connection_pool *pool,
                                    MYSQL *mysql_client) {
  mysql_connection_element *new_element =
      (mysql_connection_element *)malloc(sizeof(mysql_connection_element));
  new_element->before_ptr = NULL;
  new_element->mysql_client = mysql_client;

  pthread_mutex_lock(&pool->lock);
  pool->current_size++;
  new_element->label_num = pool->current_size;
  printf("============ mysql_client appending... label:%d\n",
         new_element->label_num);
  if (pool->last_ptr == NULL) {
    pool->last_ptr = new_element;

    // この瞬間はconnectionが1つでも利用可能なので、利用可能な状態のsignalを送る
    printf("============== send signal...! NULLから1つ利用可能に\n");
    pthread_cond_signal(&pool->exists_available_connection);
    pthread_mutex_unlock(&pool->lock);
    return;
  }

  new_element->before_ptr = pool->last_ptr;
  pool->last_ptr = new_element;

  // この瞬間はconnectionが1つでも利用可能なので、利用可能状態のsignalを送る
  printf("=========== send signal...!\n");
  pthread_cond_signal(&pool->exists_available_connection);
  pthread_mutex_unlock(&pool->lock);
}

extern MYSQL *pop_mysql_connection(mysql_connection_pool *pool) {
  if (pool == NULL) {
    printf("======== mysql_connections is NULL\n");
    return NULL;
  }

  pthread_mutex_lock(&pool->lock);
  while (pool->last_ptr == NULL) {
    printf(
        "======= mysql "
        "connectionが枯渇しているので、connectionが空くまで待機します\n");
    pthread_cond_wait(&pool->exists_available_connection, &pool->lock);
  }

  pool->current_size--;

  mysql_connection_element *pop_element = pool->last_ptr;
  pool->last_ptr = pool->last_ptr->before_ptr;
  MYSQL *pop_mysql_client = pop_element->mysql_client;

  pop_element->mysql_client = NULL;
  printf("============ mysql_client pop label_num:%d\n",
         pop_element->label_num);
  pop_element->label_num = 0;
  free(pop_element);
  pthread_mutex_unlock(&pool->lock);
  return pop_mysql_client;
}

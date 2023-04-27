#include <mysql.h>
#include <pthread.h>

#include "../mysql.h"

typedef struct _mysql_connection_element {
  struct _mysql_connection_element *before_ptr;
  MYSQL *mysql_client;
  int label_num;
} mysql_connection_element;

typedef struct {
  size_t max_size;
  size_t current_size;
  mysql_connection_element *last_ptr;
  pthread_mutex_t lock;
  pthread_cond_t
      exists_available_connection;  
} mysql_connection_pool;

extern mysql_connection_pool *new_mysql_connection_pool(mysql_config *config,
                                                        int connection_count);
extern void mysql_connection_pool_destroy(mysql_connection_pool *pool);
extern void append_mysql_connection(mysql_connection_pool *pool,
                                    MYSQL *mysql_client);
extern MYSQL *pop_mysql_connection(mysql_connection_pool *pool);

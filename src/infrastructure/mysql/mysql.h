#ifndef INFRA_MYSQL
#define INFRA_MYSQL

#include <mysql.h>

typedef struct _mysql_config {
  char *host;
  char *user;
  char *pass;
  char *database;
  int port;
} mysql_config;

extern MYSQL *mysql_setup(mysql_config *config);
extern void mysql_connection_destroy(MYSQL *connection);

#endif

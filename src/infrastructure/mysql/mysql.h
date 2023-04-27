#ifndef INFRA_MYSQL
#define INFRA_MYSQL

#include <mysql.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mysql_config {
  char *host;
  char *user;
  char *pass;
  char *database;
  int port;
} mysql_config;

extern MYSQL *mysql_setup(mysql_config *config);
extern void mysql_connection_destroy(MYSQL *connection);

#ifdef __cplusplus
}
#endif

#endif

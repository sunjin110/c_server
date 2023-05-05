#include "mysql.h"

#include <stdlib.h>

extern MYSQL *mysql_setup(mysql_config *config) {
  if (config == NULL) {
    return NULL;
  }
  MYSQL *connection = mysql_init(NULL);
  mysql_real_connect(connection, config->host, config->user, config->pass, config->database, config->port, NULL, 0);
  return connection;
}

extern void mysql_connection_destroy(MYSQL *connection) {
  if (connection == NULL) {
    return;
  }
  mysql_close(connection);
}

#include "mysql.h"

#include <stdio.h>
#include <stdlib.h>

extern MYSQL *mysql_setup(mysql_config *config) {
  if (config == NULL) {
    return NULL;
  }
  MYSQL *connection = mysql_init(NULL);
  // return mysql_real_connect(connection, config->host, config->user,
  // config->pass, config->database, config->port, NULL, 0);
  mysql_real_connect(connection, config->host, config->user, config->pass,
                     config->database, config->port, NULL, 0);

  // const char* err_message = mysql_error(connection);
  // if (err_message != NULL) {
  //     printf("host: %s, user: %s, pass: %s, database: %s, port: %d\n",
  //     config->host, config->user, config->pass, config->database,
  //     config->port); printf("error message is %s\n", err_message);
  // }

  return connection;
}

extern void mysql_connection_destroy(MYSQL *connection) {
  if (connection == NULL) {
    return;
  }
  mysql_close(connection);
}

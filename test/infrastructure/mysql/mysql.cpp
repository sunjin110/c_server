
#include "mysql.h"

#include <stdlib.h>

extern mysql_config *test_mysql_config() {
  mysql_config *cfg = (mysql_config *)malloc(sizeof(mysql_config));
  cfg->database = "c_server";
  cfg->user = "c_server_user";
  cfg->pass = "passw0rd";
  // cfg->host = "127.0.0.1";
  cfg->host = "mariadb";
  //   cfg->port = 13306;
  cfg->port = 3306;
  return cfg;
}

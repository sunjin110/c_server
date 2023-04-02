#include "migration.h"

#include <mysql.h>

extern int mysql_migration(ctx *c) {
  MYSQL *conn = c->mysql_connection;
  int result;
  result = mysql_query(
      conn,
      "CREATE TABLE IF NOT EXISTS users(id VARCHAR(37) NOT NULL, name "
      "VARCHAR(255) NOT NULL, age INT(3) NOT NULL, PRIMARY KEY (id));");
  if (result != 0) {
    return result;
  }

  result = mysql_query(
    conn,
    "ALTER TABLE users ADD (email varchar(256), password varchar(256));"
  );
  if (result != 0) {
    return result;
  }

  return 0;
}


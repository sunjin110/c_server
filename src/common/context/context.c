
#include "context.h"

extern ctx *new_ctx(MYSQL *mysql_connection) {
  ctx *c = malloc(sizeof(c));
  c->mysql_connection = mysql_connection;
  return c;
}

extern void destroy_ctx(ctx *c) {
  if (c == NULL) {
    return;
  }

  if (c->mysql_connection == NULL) {
    c = NULL;
    return;
  }

  mysql_close(c->mysql_connection);
  c->mysql_connection = NULL;
  free(c);
  c = NULL;
}

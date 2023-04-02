#ifndef COMMON_CONTEXT
#define COMMON_CONTEXT

#include <mysql.h>

// ctxの中身は必ずdomain内では利用しない
// どうしても依存してしまうような事柄を追加する

typedef struct _ctx {
  MYSQL *mysql_connection;
} ctx;

extern ctx *new_ctx(MYSQL *mysql_connection);
extern void destroy_ctx(ctx *c);

#endif
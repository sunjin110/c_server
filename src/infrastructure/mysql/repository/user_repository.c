#include "user_repository.h"
#include "../template/insert_user.sql.h"
#include "../template/delete_user.sql.h"
#include "../template/get_user.sql.h"

#include <mysql.h>
#include <jansson.h>
#include <stdlib.h>
#include "../../../../lib/mustach/mustach-jansson.h"

// TODO MYSQL *connのスレッドセーフ
// httpのthreadごとにconnectionを作成するか
// MYSQLのconnectionプールを作成する方法のどちらかをする必要がある

extern user_repository *new_user_repository_for_mysql() {
  user_repository *repo = malloc(sizeof(user_repository));
  repo->insert_user = insert_user;
  repo->delete_user = delete_user;
  repo->get_user = get_user;
  return repo;
}

static int insert_user(ctx *c, user *insert_user) {
  MYSQL *conn = c->mysql_connection;
  json_t *context = json_pack("{s:s,s:s,s:i,s:s,s:s}",
   "id", insert_user->id,
   "name", insert_user->name,
   "age", insert_user->age,
   "email", insert_user->email,
   "password", insert_user->password);

  char *query = NULL;
  size_t query_size = 0;
  int result_status = mustach_jansson_mem((const char *)insert_user_sql, insert_user_sql_len, context, 0, &query, &query_size);
  json_decref(context);
  if (result_status < 0) {
    printf("=========== user_repository insert_user failed template\n");
    return -1;
  }

  result_status = mysql_query(conn, query);
  free(query);
  if (result_status != 0) {
    printf("========== user_repository insert_user exec query\n");
    const char *error_message = mysql_error(conn);
    printf("============== error_message is %s\n", error_message);
    free((void*)error_message);
    return -1;
  }

  return 0;
}

static int delete_user(ctx *c, const char *id) {
  json_t *context = json_pack("{s:s}", "id", id);

  char *query = NULL;
  size_t query_size = 0;
  int result_status = mustach_jansson_mem((const char *)delete_user_sql, delete_user_sql_len, context, 0, &query, &query_size);
  json_decref(context);
  if (result_status < 0) {
    printf("=============== user_repository delete_user failed template\n");
    return -1;
  }
  result_status = mysql_query(c->mysql_connection, query);
  free(query);
  if (result_status != 0) {
    printf("=========== user_repository delete_user failed exec query\n");
    const char *err_msg = mysql_error(c->mysql_connection);
    printf("============ err_msg is %s\n", err_msg);
    free((void *)err_msg);
    return -1;
  }
  return 0;
}

static user *get_user(ctx *c, const char *id) { 
  json_t *context = json_pack("{s:s}", "id", id);

  char *query = NULL;
  size_t query_size = 0;
  int result_status = mustach_jansson_mem((const char *)get_user_sql, get_user_sql_len, context, 0, &query, &query_size);
  json_decref(context);
  if (result_status != 0) {
    printf("=================== user_repository get_user failed template\n");
    return NULL;
  }

  printf("=========== get query is %s\n", query);

  result_status = mysql_query(c->mysql_connection, query);
  free(query);
  if (result_status != 0) {
    printf("=========== user_repository get_user failed exec query\n");
    const char *err_msg = mysql_error(c->mysql_connection);
    printf("============ err_msg is %s\n", err_msg);
    free((void *)err_msg);
    return NULL;
  }

  MYSQL_RES *resp = mysql_use_result(c->mysql_connection);

  // 必ず1つのためroopはさせない
  MYSQL_ROW row = mysql_fetch_row(resp);
  if (row == NULL) {
    printf("=========== target not found");
    mysql_free_result_nonblocking(resp);  
    return NULL;
  }

  user *u = user_of(row[0], row[1], atoi(row[2]), row[3], row[4]);

  mysql_free_result_nonblocking(resp);
  return u; 
}

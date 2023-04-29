#include <gtest/gtest.h>

#include <iostream>

#include "../migration/migration.h"
#include "../mysql.h"

extern "C" {
#include "../../../../src/common/context/context.h"
#include "../../../../src/infrastructure/mysql/migration/migration.h"
#include "../../../../src/infrastructure/mysql/mysql.h"
#include "../../../../src/infrastructure/mysql/repository/user_repository.h"
#include "../../../../src/repository/user_repository.h"
#include "../../../../src/utils/uuid/uuid.h"
}

// ./build/c_server_test --gtest_filter=insert_user_test.success
TEST(insert_user_test, success) {
  std::cout << "==== insert_user test ====\n";
  mysql_config *cfg = test_mysql_config();
  std::cout << "mysql setup \n";
  MYSQL *conn = mysql_setup(cfg);

  ctx *c = new_ctx(conn);
  std::cout << "=== new user repo  \n";
  user_repository *repo = new_user_repository_for_mysql();

  if (repo == NULL) {
    std::cout << "repo is NULL\n";
    FAIL();
    return;
  }

  user *insert_user = new_user("sunjin", 123, "email", "pass");

  std::cout << "== insert user\n";
  int result = repo->insert_user(c, insert_user);
  printf("user_insert result is %d\n", result);
  destroy_ctx(c);
}

// ./build/c_server_test --gtest_filter=delete_user_test.success
TEST(delete_user_test, success) {
  mysql_config *cfg = test_mysql_config();
  std::cout << "mysql setup \n";
  MYSQL *conn = mysql_setup(cfg);
  ctx *c = new_ctx(conn);
  user_repository *repo = new_user_repository_for_mysql();
  user *insert_user = new_user("sunjin", 123, "email", "pass");
  int result = repo->insert_user(c, insert_user);
  printf("user_insert result is %d\n", result);

  result = repo->delete_user(c, insert_user->id);
  printf("user_delete result is %d\n", result);
  destroy_ctx(c);
}

// ./build/c_server_test --gtest_filter=get_user_test.success
TEST(get_user_test, success) {
  mysql_config *cfg = test_mysql_config();
  std::cout << "mysql setup \n";
  MYSQL *conn = mysql_setup(cfg);
  ctx *c = new_ctx(conn);
  user_repository *repo = new_user_repository_for_mysql();
  user *insert_user = new_user("sunjin", 123, "email", "pass");
  int result = repo->insert_user(c, insert_user);
  printf("user_insert result is %d\n", result);

  user *u = repo->get_user(c, insert_user->id);
  if (u == NULL) {
    FAIL();
    return;
  }
  debug_user_print(u);
  destroy_ctx(c);
}

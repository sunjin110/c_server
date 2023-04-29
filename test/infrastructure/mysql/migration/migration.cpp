#include <errno.h>
#include <gtest/gtest.h>

#include <iostream>

#include "../mysql.h"

extern "C" {
#include "../../../../src/common/context/context.h"
#include "../../../../src/infrastructure/mysql/migration/migration.h"
#include "../../../../src/infrastructure/mysql/mysql.h"
}

extern void setup_mysql() {
  mysql_config *cfg = test_mysql_config();

  MYSQL *conn = mysql_setup(cfg);
  if (conn == NULL) {
    std::cout << "mysql connectionの作成に失敗しました\n";
    std::cout << "======= errno is " << errno << "\n";
    FAIL();
  }

  ctx c = ctx{
      .mysql_connection = conn,
  };

  int result = mysql_migration(&c);
  mysql_close(conn);
  printf("result is %d\n", result);
}

// ./build/c_server_test --gtest_filter=migration_test.success
TEST(migration_test, success) {
  std::cout << "==== migration test ====\n";
  mysql_config *cfg = test_mysql_config();

  MYSQL *conn = mysql_setup(cfg);
  if (conn == NULL) {
    std::cout << "mysql connectionの作成に失敗しました\n";
    std::cout << "======= errno is " << errno << "\n";
    FAIL();
  }

  ctx *c = new_ctx(conn);

  int result = mysql_migration(c);
  printf("result is %d\n", result);
  destroy_ctx(c);
}

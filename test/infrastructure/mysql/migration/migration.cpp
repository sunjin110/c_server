#include <gtest/gtest.h>
#include <iostream>
#include <errno.h>

extern "C" {
    #include "../../../../src/infrastructure/mysql/migration/migration.h"
    #include "../../../../src/infrastructure/mysql/mysql.h"
    #include "../../../../src/common/context/context.h"
}

extern void setup_mysql() {
  mysql_config cfg = {
        .database = "c_server",
        .user = "c_server_user",
        .pass = "passw0rd",
        .host = "127.0.0.1",
        .port = 13306,
    };

    MYSQL *conn = mysql_setup(&cfg);
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

TEST(migration_test, success) {
    std::cout << "==== migration test ====\n";

    mysql_config cfg = {
        .database = "c_server",
        .user = "c_server_user",
        .pass = "passw0rd",
        .host = "127.0.0.1",
        .port = 13306,
    };

    MYSQL *conn = mysql_setup(&cfg);
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

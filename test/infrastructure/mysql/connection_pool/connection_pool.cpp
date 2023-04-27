#include <gtest/gtest.h>
#include <iostream>
#include "../mysql.h"
#include <unistd.h>
#include <pthread.h>

extern "C" {
    #include "../../../../src/infrastructure/mysql/connection_pool/connection_pool.h"
    #include "../../../../src/infrastructure/mysql/mysql.h"
}

void *append_new_connection_to_pool(void *p) {

    mysql_connection_pool *pool = (mysql_connection_pool *)p;
    mysql_config *cfg = test_mysql_config();
    MYSQL *new_mysql_client = mysql_setup(cfg);
    if (new_mysql_client == NULL) {
        std::cout << "===== other thread ===== failed create mysql client...\n";
        return NULL;
    }

    std::cout << "===== other thread ===== wait 1\n";
    sleep(1);
    std::cout << "===== other thread ===== wait 2\n";
    sleep(1);
    std::cout << "===== other thread ===== wait 3\n";
    sleep(1);

    std::cout << "===== other thread ===== append new mysql client\n";    
    append_mysql_connection(pool, new_mysql_client);
    return NULL;
}

TEST(connection_pool, success) {
    std::cout << "==== mysql_connection_pool test ====\n";

    mysql_config *cfg = test_mysql_config();

    // 10個のconnectionを作ってみる
    mysql_connection_pool *pool = new_mysql_connection_pool(cfg, 3);
    if (pool == NULL) {
        std::cout << "======= connection poolがNULL";
        FAIL();
    }

    // other thread
    pthread_t other_thread;
    pthread_create(&other_thread, NULL, append_new_connection_to_pool, (void *)pool);

    // pop
    MYSQL *mysql_client = pop_mysql_connection(pool);
    MYSQL *mysql_client2 = pop_mysql_connection(pool);
    MYSQL *mysql_client3 = pop_mysql_connection(pool);

    std::cout << "=========== ここでlockがかかる\n";
    MYSQL *mysql_client4 = pop_mysql_connection(pool);
    std::cout << "=========== waitが解除された\n";
    
    pthread_join(other_thread, NULL);

    // 戻す
    append_mysql_connection(pool, mysql_client);
    append_mysql_connection(pool, mysql_client2);
    append_mysql_connection(pool, mysql_client3);
    append_mysql_connection(pool, mysql_client4);

    // connectionsを削除
    mysql_connection_pool_destroy(pool);
}

// void append_new_connection_to_pool(mysql_connection_pool *pool) {
// void *append_new_connection_to_pool(void *p) {

//     mysql_connection_pool *pool = (mysql_connection_pool *)p;
//     mysql_config *cfg = test_mysql_config();
//     MYSQL *new_mysql_client = mysql_setup(cfg);
//     if (new_mysql_client == NULL) {
//         std::cout << "========== failed create mysql client...\n";
//         return NULL;
//     }

//     std::cout << "========== wait 1\n";
//     sleep(1);
//     std::cout << "========== wait 2\n";
//     sleep(1);
//     std::cout << "========== wait 3\n";
//     sleep(1);

//     std::cout << "========== append new mysql client\n";    
//     append_mysql_connection(pool, new_mysql_client);
//     return NULL;
// }
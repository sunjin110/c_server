#include <gtest/gtest.h>

extern "C" {
    #include "../../../src/common/logger/logger.h"
}

// #define DEBUG_MODE

// make b && ./build/c_server_test --gtest_filter=src_common_logger_DEBUG_PRINT.success
TEST(src_common_logger_DEBUG_PRINT, success) {
    printf("========== debug print test \n");
    DEBUG_PRINT("=========== DEBUG_PRINT ========== %s\n", "hoge");

    DEBUG_PRINTLN("hoge");
    printf("=== aaa\n");
}

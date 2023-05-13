#include <gtest/gtest.h>

extern "C" {
   #include "../../../src/infrastructure/http/static.h"
}

// make b && ./build/c_server_test --gtest_filter=src_infrastructure_http_static.walk_test
TEST(src_infrastructure_http_static, walk_test) {
    walk_test();
}


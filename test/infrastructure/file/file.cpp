#include <gtest/gtest.h>

extern "C" {
#include "../../../src/infrastructure/file/file.h"
}

// ./build/c_server_test --gtest_filter=src_infrastructure_file.success
TEST(src_infrastructure_file, success) {
  char *content = get_file_content(
      "/Users/yun_sunjin/develop/c_server/test/infrastructure/file/"
      "testfile.txt");
  printf("content is [%s]\n", content);
}

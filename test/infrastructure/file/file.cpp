#include <gtest/gtest.h>

extern "C" {
     #include "../../../src/infrastructure/file/file.h"
}

TEST(src_infrastructure_file, success) {
    char *content = get_file_content("/Users/yun_sunjin/develop/c_server/test/infrastructure/file/testfile.txt");
    printf("content is [%s]\n", content);
}

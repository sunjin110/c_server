#include <gtest/gtest.h>
#include <stdlib.h>

extern "C" {
    #include "../../../src/common/config/config.h"
}

TEST(src_common_config_setup, default_config_value) {
    config c = setup();
    EXPECT_EQ(8088, c.port);
}

TEST(src_common_config, config_value_from_env) {
    int result = setenv("PORT", "1111", 1);
    if (result == -1) {
        printf("test failed: 環境変数の設定に失敗\n");
        FAIL();
    }
    config c = setup();
    EXPECT_EQ(1111, c.port);
}

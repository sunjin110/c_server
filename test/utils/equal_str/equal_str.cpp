#include <gtest/gtest.h>
#include <stdio.h>
#include <stdbool.h>

extern "C" {
    #include "../../../src/utils/equal_str/equal_str.h"
}

TEST(src_utils_equal_str, success) {

    char *test_str = (char *)"test_string";

    ASSERT_EQ(equal_str(test_str, "test_string"), true);
    ASSERT_EQ(equal_str(test_str, "test_string_not"), false);
}

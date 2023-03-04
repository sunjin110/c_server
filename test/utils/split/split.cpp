#include <gtest/gtest.h>

extern "C" {
    #include "../../../src/utils/split/split.h"
}

TEST(src_utils_split, success) {
    split("sunjin,hoge,222,333", ",");
} 

#include <gtest/gtest.h>
#include <string>

extern "C" {
    #include "../../../src/utils/str/str.h"
}

// make b && ./build/c_server_test --gtest_filter=src_utils_str.has_prefix
TEST(src_utils_str, has_prefix) {
    struct args {
        const char *pre;
        const char *str;
    };

    struct test {
        std::string name;
        args args;
        bool want;
    };

    const struct test tests[] = {
        {   
            .name = "When include prefix in string, then return true",
            .args = args{
                .pre  = "abc",
                .str = "abcdefg"
            },
            .want = true,
        },
        {
            .name = "When not include prefix in string, then return false",
            .args = args{
                .pre = "123",
                .str = "abcdefg",
            },
            .want = false,
        },
        {
            .name = "When prefix is NULL, then return false",
            .args = args{
                .pre = NULL,
                .str = "abcdef",
            },
            .want = false
        },
        {
            .name = "When string is NULL, then return false",
            .args = args{
                .pre = "abc",
                .str = NULL,
            },
            .want = false
        },
        {
            .name = "When prefix and string is equal, then return true",
            .args = args{
                .pre = "sunjin",
                .str = "sunjin",
            },
            .want = true,
        },
    };

    for (test t : tests) {
        std::cout << "str.c has_prefix : " << t.name << std::endl;
        bool got = has_prefix(t.args.pre, t.args.str);
        ASSERT_EQ(got, t.want);
    }
}

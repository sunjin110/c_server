#include <gtest/gtest.h>
#include <stdio.h>

extern "C" {
    #include "../../../src/utils/uri/uri.h"
}

TEST(src_utils_uri_encode_uri, success) {

    char *test_str = "やすな";

    char *result_str = NULL;
    int result = encode_uri(test_str, &result_str);
    ASSERT_STREQ(result_str, "\%E3\%82\%84\%E3\%81\%99\%E3\%81\%AA");
    ASSERT_EQ(result, 0);
    free(result_str);
}

TEST(src_utils_uri_decode_uri, success) {

    char *test_str = "\%E3\%82\%84\%E3\%81\%99\%E3\%81\%AA";

    char *result_str = NULL;
    int result = decode_uri(test_str, &result_str);
    ASSERT_STREQ(result_str, "やすな");
    ASSERT_EQ(result, 0);

    printf("result_str is %s\n", result_str);
    free(result_str);
}


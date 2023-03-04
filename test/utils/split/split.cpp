#include <gtest/gtest.h>

extern "C" {
    #include "../../../src/utils/split/split.h"
    #include "../../../src/utils/linked_str_list/linked_str_list.h"
}

TEST(src_utils_split, success) {
    linked_str_list *list = split("sunjin,hoge,222,333", ",");
    print_linked_str_list(list);
    ASSERT_STREQ("sunjin", value_linked_str_list(list, 0));
    ASSERT_STREQ("hoge", value_linked_str_list(list, 1));
    ASSERT_STREQ("222", value_linked_str_list(list, 2));
    ASSERT_STREQ("333", value_linked_str_list(list, 3));
} 

TEST(str_utils_split, get_before_str) {
    char *result = get_before_str("sunjin,yunjfdksla;jdkl;sjdfkldsjal;kjklsdjfkjsdklfjdsl;jalk;jsdklfjkl;jsdkjflksjkajklsjdkfjkjkjkjkjkjkj", ",");
    ASSERT_STREQ(result, "sunjin");
}

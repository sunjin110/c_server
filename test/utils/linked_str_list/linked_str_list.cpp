#include <gtest/gtest.h>
#include <stdio.h>

extern "C" {
    #include "../../../src/utils/linked_str_list/linked_str_list.h"
}

TEST(src_utils_linked_str_list, success) {

    std::cout << "===== new_linked_str_list\n";
    linked_str_list *list = new_linked_str_list();

    std::cout << "===== append_linked_str_list\n";
    append_linked_str_list(list, "hello world");
    append_linked_str_list(list, "sunjin");
    append_linked_str_list(list, "どうぞよろしく");
    std::cout << "===== print_linked_str_list\n";
    print_linked_str_list(list);

    ASSERT_STREQ(value_linked_str_list(list, 0), "hello world");
    ASSERT_STREQ(value_linked_str_list(list, 1), "sunjin");
    ASSERT_STREQ(value_linked_str_list(list, 2), "どうぞよろしく");
}

#include <gtest/gtest.h>
#include <iostream>

extern "C" {
    #include "../../../src/utils/uuid/uuid.h"
}

TEST(utils_uuid_new_uuid, success) {
    const char *uuid = new_uuid();
    std::cout << "uuid is " << uuid << "\n";
    free((void*)uuid);
}

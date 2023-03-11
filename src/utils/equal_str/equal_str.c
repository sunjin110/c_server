#include <stdbool.h>
#include "string.h"

extern bool equal_str(const char *str_1, const char *str_2) {

    if (str_1 == NULL) {
        return str_2 == NULL;
    }

    if (str_2 == NULL) {
        return false;
    }

    return strcmp(str_1, str_2) == 0;
}

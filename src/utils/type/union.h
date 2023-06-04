#ifndef _UNION_
#define _UNION_


#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../../common/logger/logger.h"

typedef enum _comparable_union_enum {
    COMPARABLE_UNION_ENUM_NUMBER,
    COMPARABLE_UNION_ENUM_STRING,
    COMPARABLE_UNION_ENUM_CHARACTER,
} comparable_union_enum;

typedef struct _comparable_union_t {
    int num;
    const char *string;
    char c;
    enum _comparable_union_enum type;
} comparable_union_t;

extern comparable_union_t *new_comparable_union(comparable_union_enum type, int num, const char *str, char c);
extern void free_comparable_union(comparable_union_t *comparable_union);

#endif
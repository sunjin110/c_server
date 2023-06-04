#include "union.h"

extern comparable_union_t *new_comparable_union(comparable_union_enum type, int num, const char *str, char c) {
    comparable_union_t *comparable_union = (comparable_union_t*)malloc(sizeof(comparable_union_t));
    if (comparable_union == NULL) {
        ERR_PRINTLN("failed malloc comparable_union");
        return NULL;
    }

    comparable_union->type = type;

    if (type == COMPARABLE_UNION_ENUM_STRING) {
        comparable_union->string = strdup(str);
        return comparable_union;
    }

    if (type == COMPARABLE_UNION_ENUM_NUMBER) {
        comparable_union->num = num;
        return comparable_union;
    }

    comparable_union->c = c;
    return comparable_union;
}

extern void free_comparable_union(comparable_union_t *comparable_union) {
    if (comparable_union == NULL) {
        DEBUG_PRINTLN("comparable_union is NULL");
        return;
    }

    if (comparable_union->string != NULL) {
        free((void *)comparable_union->string);
        comparable_union->string = NULL;
    }

    comparable_union->num = 0;
    comparable_union->c = '\0';
    free(comparable_union);
}

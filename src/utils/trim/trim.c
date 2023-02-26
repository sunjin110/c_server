#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *ltrim(char *str, const char *seps) {
    if (seps == NULL) {
        seps = "\t\n\v\f\r ";
    }
    size_t to_trim = strspn(str, seps);
    if (to_trim > 0) {
        size_t len = strlen(str);
        if (to_trim == len) {
            str[0] = '\0';
        } else {
            memmove(str, str + to_trim, len + 1 - to_trim);
        }
    }
    return str;
}

char *rtrim(char *str, const char *seps) {
    if (seps == NULL) {
        seps = "\t\n\v\f\r ";
    }
    int i = strlen(str) - 1;
    while (i >= 0 && strchr(seps, str[i]) != NULL) {
        str[i] = '\0';
        i--;
    }
    return str;
}

extern char *trim(char *str, const char *seps) {
    return ltrim(rtrim(str, seps), seps);
}


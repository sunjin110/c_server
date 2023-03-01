#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void split(const char *str, const char *sep) {
    int str_len = strlen(str);
    char target_str[str_len + 1];
    strncpy(target_str, str, str_len);

    char *rest = NULL;
    char *token = NULL;

    token = strtok_r(target_str, sep, &rest);
    if (token == NULL) {
        return;
    }

    printf("token = %s\n", token);

    for (;;) {
        token = strtok_r(NULL, sep, &rest);    
        if (token == NULL) {
            break;
        }
        printf("token = %s\n", token);
    }
    free(rest);
    free(token);
}

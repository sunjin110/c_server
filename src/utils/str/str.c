#include <stdio.h>

extern void print_char_list(const char *str) {
    printf("[");
    for (;;) {
        char c = *str;
        if (c == '\0') {
            printf("\\0");
            break;
        }
        printf("%c,", c);

        str++;
    }
    printf("]\n");
}

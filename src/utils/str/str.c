#include <stdio.h>
#include <string.h>
#include "str.h"

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

extern bool has_prefix(const char *pre, const char *str) {
  if (pre == NULL || str == NULL) {
    return false;
  }

  return strncmp(pre, str, strlen(pre)) == 0;
}

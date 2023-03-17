#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../linked_str_list/linked_str_list.h"

extern linked_str_list *split(const char *str, const char *sep) {
  if (str == NULL) {
    return NULL;
  }

  size_t str_len = strlen(str);
  char target_str[str_len + 1];
  strncpy(target_str, str, str_len);
  target_str[str_len] = '\0';

  char *rest = NULL;
  char *token = NULL;

  token = strtok_r(target_str, sep, &rest);
  if (token == NULL) {
    return NULL;
  }

  linked_str_list *list = new_linked_str_list();
  append_linked_str_list(list, token);

  for (;;) {
    token = strtok_r(NULL, sep, &rest);
    if (token == NULL) {
      break;
    }
    append_linked_str_list(list, token);
  }

  if (rest != NULL) {
    free(rest);
  }

  return list;
}

char *get_before_str(const char *str, const char *pattern) {
  char *target_str = strdup(str);
  if (target_str == NULL) {
    return NULL;
  }

  char *find_ptr = strstr(target_str, pattern);
  if (find_ptr == NULL) {
    free(target_str);
    return NULL;
  }
  *find_ptr = '\0';
  char *new_target_str = realloc(target_str, strlen(target_str) + 1);
  if (new_target_str == NULL) {
    free(target_str);
    return NULL;
  }
  return new_target_str;
}

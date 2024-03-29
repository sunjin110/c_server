#include "linked_str_list.h"

#include <memory.h>
#include <stdlib.h>
#include <string.h>

#include "../../common/logger/logger.h"

extern linked_str_list *new_linked_str_list() {
  linked_str_list *list = malloc(sizeof(linked_str_list));
  list->size = 0;
  list->first_ptr = NULL;
  list->last_ptr = NULL;
  return list;
}

extern void append_linked_str_list(linked_str_list *list, char *value) {
  list->size++;

  linked_str_element *new_element = malloc(sizeof(linked_str_element));
  new_element->value = strdup(value);
  new_element->next_ptr = NULL;
  if (list->first_ptr == NULL) {
    list->first_ptr = new_element;
    list->last_ptr = new_element;
    return;
  }
  list->last_ptr->next_ptr = new_element;
  list->last_ptr = new_element;
}

extern char *value_linked_str_list(linked_str_list *list, size_t index) {
  size_t max_index = list->size - 1;
  if (max_index < index) {
    ERR_PRINTLN("linked_str_list: out of bounds. index=%zu", index);
    return NULL;
  }

  linked_str_element *current_element = list->first_ptr;
  size_t current_index = 0;
  for (;;) {
    if (current_element == NULL) {
      return NULL;
    }

    if (current_index == index) {
      return strdup(current_element->value);
    }
    current_element = current_element->next_ptr;
    current_index++;
  }
  return NULL;
}

extern void free_linked_str_list(linked_str_list *list) {
  linked_str_element *current_element = list->first_ptr;
  for (;;) {
    if (current_element == NULL) {
      break;
    }
    linked_str_element *tmp_ptr = current_element->next_ptr;
    free(current_element->value);
    current_element->value = NULL;
    free(current_element);
    current_element = tmp_ptr;
  }
  list->first_ptr = NULL;
  list->last_ptr = NULL;
  list->size = 0;
  free(list);
  list = NULL;
}

extern bool find_in_linked_str_list(linked_str_list *list, char *target_value) {
  linked_str_element *current_element = list->first_ptr;
  for (;;) {
    if (current_element == NULL) {
      return false;
    }
    if (strcmp(current_element->value, target_value) == 0) {
      return true;
    }
    current_element = current_element->next_ptr;
  }
  return false;
}

extern void print_linked_str_list(linked_str_list *list) {
  linked_str_element *current_element = list->first_ptr;
  DEBUG_PRINT("[");
  for (;;) {
    if (current_element == NULL) {
      return;
    }
    if (current_element->next_ptr == NULL) {
      DEBUG_PRINT("%s]\n", current_element->value);
      return;
    }
    DEBUG_PRINT("%s,", current_element->value);
    current_element = current_element->next_ptr;
  }
}

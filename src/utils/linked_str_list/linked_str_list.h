#include <stdbool.h>

typedef struct _linked_str_list {
    struct _linked_str_element *first_ptr;
    struct _linked_str_element *last_ptr;
    size_t size;
} linked_str_list;

typedef struct _linked_str_element {
    struct _linked_str_element *next_ptr;
    char *value;
} linked_str_element;

extern linked_str_list* new_linked_str_list(void); 

extern void append_linked_str_list(linked_str_list *list, char *value);
extern bool find_in_linked_str_list(linked_str_list *list, char *target_value);
extern void free_linked_str_list(linked_str_list *list);
extern void print_linked_str_list(linked_str_list *list);

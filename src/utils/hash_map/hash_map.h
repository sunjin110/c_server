#include <stdio.h>

typedef struct _entry {
    char *key;
    char *value;
    struct _entry *next;
} entry;

typedef struct _hash_map {
    struct _entry **buckets;
    ssize_t size;
    int capacity;
} hash_map;



extern hash_map *new_hash_map(void);
extern void free_hash_map(hash_map *map);
extern char *get_value_from_hash_map(hash_map *map, const char *key);
extern void put_to_hash_map(hash_map *map, const char *key, const char *value);

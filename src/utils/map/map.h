#ifndef _MAP_
#define _MAP_

#include <stdio.h>
#include <stdlib.h>

#include "../type/union.h"

typedef struct _map_entry_t {
    void *key; // only int or string or c
    void *value; // any
    struct _map_entry_t *next;
} map_entry_t;

typedef struct _map_t {
    struct _map_entry_t **buckets;
    ssize_t size;
    int capacity;

    comparable_union_enum key_type;

    void *(*get)(struct _map_t *self, const void *key);
    void (*put)(struct _map_t *self, void *key, void *value);
    void (*remove)(struct _map_t *self, const void *key);
} map_t;

extern map_t *new_map(comparable_union_enum key_type);
extern void free_map(map_t *m);

static void *get_value_from_map(map_t *self, const void *key);

// ここのvalueはそのまま格納される
static void put_value_to_map(map_t *self, void *key, void *value);
static void *default_remove_value_from_map(map_t *self, const void *key);

static uint64_t hash_for_map(const comparable_union_t *key, int capacity);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int INITIAL_CAPACITY = 16;

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

static int hash(const char *key, int capacity) {
    int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash = 31 * hash + key[i];
    }
    return hash % capacity;
}

extern hash_map *new_hash_map() {
    hash_map *map = malloc(sizeof(hash_map));
    if (map == NULL) {
        printf("Error: failed malloc hash_map\n");
        return NULL;
    }

    map->buckets = calloc(INITIAL_CAPACITY, sizeof(entry*));
    if (map->buckets == NULL) {
        printf("Error: failed malloc hash_map->buckets\n");
        return NULL;
    }

    map->size = 0;
    map->capacity = INITIAL_CAPACITY;
    return map;
}

extern void free_hash_map(hash_map *map) {
    for (int i = 0; i < map->capacity; i++) {
        entry *current_entry = map->buckets[i];
        for (;;) {
            if (current_entry == NULL) {
                break;
            }
            free(current_entry->key);
            free(current_entry->value);
            free(current_entry);
            current_entry = current_entry->next;
        }
    }
    free(map->buckets);
    free(map);
}

static entry *get_entry_from_hash_map(hash_map *map, const char *key) {
    int index = hash(key, map->capacity);
    entry *current_entry = map->buckets[index];

    for (;;) {
        if (current_entry == NULL) {
            return NULL;
        }
        if (strcmp(current_entry->key, key) == 0) {
            return current_entry;
        }
        current_entry = current_entry->next;
    }
    return NULL;
}

extern char *get_value_from_hash_map(hash_map *map, const char *key) {
    entry *e = get_entry_from_hash_map(map, key);
    if (e == NULL) {
        return NULL;
    }
    return e->value;
}

extern void *put_to_hash_map(hash_map *map, const char *key, const char *value) {
    // 既に対象のkeyがある場合は、valueを上書き
    entry *e = get_entry_from_hash_map(map, key);
    if (e != NULL) {
        free(e->value);
        e->value = strdup(value);
        return;
    }

    // ない場合は作成する
    int index = hash(key, map->capacity);
    entry *new_entry = malloc(sizeof(entry));
    if (new_entry == NULL) {
        printf("Error: failed malloc to new_entry\n");
        return;
    }
    new_entry->key = strdup(key);
    new_entry->value = strdup(value);
    new_entry->next = NULL;

    // 同じhash内に値がある場合は、nextをずらす
    if (map->buckets[index] != NULL) {
        new_entry->next = map->buckets[index];
    }
    map->buckets[index] = new_entry;
    map->size++;

    if (map->size <= map->capacity * 0.75) {
        return;
    }

    // TODO capacity自動で追加
    int new_capacity = map->capacity * 2;
    entry **new_buckets = calloc(new_capacity, sizeof(entry*));
    if (new_buckets == NULL) {
        printf("Error: failed calloc new_buckets\n");
        return;
    }

    for (int i = 0; i < map->capacity; i++) {
        entry *current_entry = map->buckets[i];
        for (;;) {
            if (current_entry == NULL) {
                break;
            }
            entry *tmp_next_entry = current_entry->next;
            int new_index = hash(current_entry->key, new_capacity);
            current_entry->next = new_buckets[new_index];
            new_buckets[new_index] = current_entry;
            current_entry = tmp_next_entry;
        }
    }

    free(map->buckets);
    map->buckets = new_buckets;
    map->capacity = new_capacity;
}
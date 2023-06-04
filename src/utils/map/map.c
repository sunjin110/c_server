#include "map.h"

#include "../../common/logger/logger.h"
#include "../../utils/type/union.h"
#include <stdlib.h>

static const int INITIAL_CAPACITY = 16;

static map_entry_t *get_entry_map(map_t *m, const comparable_union_t *key);
static comparable_union_t *convert_union_key(comparable_union_enum key_type, const void *key);
static uint64_t hash_for_map(const comparable_union_t *key, int capacity);

extern map_t *new_map(comparable_union_enum key_type) {
    map_t *m = malloc(sizeof(map_t));
    if (m == NULL) {
        ERR_PRINTLN("map: failed malloc map\n");
        return NULL;
    }

    m->size = 0;
    m->capacity = INITIAL_CAPACITY;
    m->key_type = key_type;

    // TODO include methods

    m->get = get_value_from_map;
    m->put = put_value_to_map;


    return m;
}

extern void free_map(map_t *m) {
    if (m == NULL) {
        DEBUG_PRINTLN("map: map already free");
        return;
    }

    for (int i = 0; i < m->capacity; i++) {
        map_entry_t *current_entry = m->buckets[i];
        for (;;) {
            if (current_entry == NULL) {
                break;
            }

            // remove value
            m->remove(m, current_entry->key);
            free(current_entry->key);
            current_entry = current_entry->next;
        }
    }
    free(m->buckets);

    // if (m->get != NULL) {
    //     free(m->get);
    // } 
    // if (m->put != NULL) {
    //     free(m->put);
    // }
    // if (m->remove != NULL) {
    //     free(m->remove);
    // }

    m->get = NULL;
    m->put = NULL;
    m->remove = NULL;
    m->capacity = 0;
    m->size = 0;
    free(m);
    m = NULL;
}


static void put_value_to_map(map_t *self, void *key, void *value) {
    comparable_union_t *union_key = convert_union_key(self->key_type, key);
    if (union_key == NULL) {
        ERR_PRINT("failed convert put key");
        return;
    }

    // 既に対象のkeyがある場合は、valueを上書きする
    map_entry_t *entry_map = get_entry_map(self, union_key);
    free_comparable_union(union_key);
    if (entry_map != NULL) {
        free(entry_map->value);
        entry_map->value = value;
        return;
    }

    // ない場合は作成する
    uint64_t index = hash_for_map(key, self->capacity);
    if (index < 0) {
        ERR_PRINTLN("failed generate index");
        return;
    }

    map_entry_t *new_entry_map = malloc(sizeof(map_entry_t));
    if (new_entry_map == NULL) {
        ERR_PRINTLN("failed malloc");
        return;
    }

    new_entry_map->key = key; // TODO deepcopyしてもいいかも
    new_entry_map->value = value;
    new_entry_map->next = NULL;

    if (self->buckets[index] != NULL) {
        // 同じhash内に値がある場合は、nextをずらす
        new_entry_map->next = self->buckets[index];
    }


    self->buckets[index] = new_entry_map;
    self->size++;

    // この後はcapacityの調整
    if (self->size <= self->capacity * 0.75) {
        // capacityの75%の容量の場合は何もしない
        return;
    }

    int new_capacity = self->capacity * 2;
    map_entry_t **new_buckets = calloc(new_capacity, sizeof(map_entry_t *));
    if (new_buckets == NULL) {
        ERR_PRINTLN("failed calloc");
        return;
    }

    for (int i = 0; i < self->capacity; i++) {
        map_entry_t *current_entry = self->buckets[i];
        for (;;) {
            if (current_entry == NULL) {
                break;
            }
            map_entry_t *tmp_next_entry = current_entry->next;
            comparable_union_t *union_key = convert_union_key(self->key_type, tmp_next_entry->key);
            uint64_t new_index = hash_for_map(union_key, new_capacity);
            current_entry->next = new_buckets[new_index];
            new_buckets[new_index] = current_entry;
            current_entry = tmp_next_entry;
            free_comparable_union(union_key);
        }
    }

    free(self->buckets);
    self->buckets = new_buckets;
    self->capacity = new_capacity;
}

static comparable_union_t *convert_union_key(comparable_union_enum key_type, const void *key) {
    switch (key_type) {
        case COMPARABLE_UNION_ENUM_STRING:
        return new_comparable_union(key_type, 0, strdup(key), '\0');

        case COMPARABLE_UNION_ENUM_NUMBER:
        return new_comparable_union(key_type, (int)key, NULL, '\0');

        case COMPARABLE_UNION_ENUM_CHARACTER:
        return new_comparable_union(key_type, 0, NULL, (char)key);
    }
    ERR_PRINTLN("not defined key type");
    return NULL;
}

static void *get_value_from_map(map_t *self, const void *key) {
    if (self == NULL) {
        return NULL;
    }

    comparable_union_t *union_key = convert_union_key(self->key_type, key);
    if (union_key == NULL) {
        ERR_PRINTLN("undefined key type");
        return NULL;
    }

    map_entry_t *map_entry = get_entry_map(self, union_key);
    free_comparable_union(union_key);
    if (map_entry == NULL) {
        return NULL;
    }

    return map_entry->value;
}

static map_entry_t *get_entry_map(map_t *map, const comparable_union_t *key) {
    uint64_t index = hash_for_map(key, map->capacity);
    if (index < 0) {
        ERR_PRINTLN("failed get index...");
        return NULL;
    }

    map_entry_t *current_entry = map->buckets[index];
    for (;;) {
        if (current_entry == NULL) {
            return NULL;
        }
        switch (key->type) {
            case COMPARABLE_UNION_ENUM_STRING:
            if (strcmp((char*)current_entry->key, key->string) == 0) {
                return current_entry;
            }
            return NULL;
            break;

            case COMPARABLE_UNION_ENUM_NUMBER:
            if ((int)current_entry->key == key->num) {
                return current_entry;
            }
            return NULL;
            break;

            case COMPARABLE_UNION_ENUM_CHARACTER:
            if ((char)current_entry->key == key->c) {
                return current_entry;
            }
            return NULL;
            break;
        }
        current_entry = current_entry->next;
    }
    return NULL;
}

static uint64_t hash_for_map(const comparable_union_t *key, int capacity) {
    if (key == NULL) {
        ERR_PRINTLN("map key is NULL");
        return -1;
    }

    if (key->type == COMPARABLE_UNION_ENUM_STRING) {
        uint64_t hash = 0;
        for (int i = 0; key->string[i] != '\0'; i++) {
            hash = (hash << 5) - hash + key->string[i];
        }
        return hash % capacity;
    }

    if (key->type == COMPARABLE_UNION_ENUM_NUMBER) {
        return key->num % capacity;
    }

    return key->c % capacity;
}

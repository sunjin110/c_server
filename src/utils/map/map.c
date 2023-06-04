#include "map.h"

#include "../../common/logger/logger.h"
#include "../../utils/type/union.h"
#include <stdlib.h>

static const int INITIAL_CAPACITY = 16;

static map_entry_t *get_entry_map(map_t *m, const comparable_union_t *key);
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

    if (m->get != NULL) {
        free(m->get);
    } 
    if (m->put != NULL) {
        free(m->put);
    }
    if (m->remove != NULL) {
        free(m->remove);
    }
    m->capacity = 0;
    m->size = 0;
    free(m);
    m = NULL;
}


static void put_value_to_map(map_t *self, const void *key, const void *value) {

    // m_entry *entry =
}

static void *get_value_from_map(map_t *self, const void *key) {
    if (self == NULL) {
        return NULL;
    }

    comparable_union_t *union_key = NULL;
    switch (self->key_type) {
        case COMPARABLE_UNION_ENUM_STRING:
        union_key = new_comparable_union(self->key_type, 0, strdup(key), '\0');
        break;

        case COMPARABLE_UNION_ENUM_NUMBER:
        union_key = new_comparable_union(self->key_type, (int)key, NULL, '\0');
        break;

        case COMPARABLE_UNION_ENUM_CHARACTER:
        union_key = new_comparable_union(self->key_type, 0, NULL, (char)key);
        break;
    }

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
    if (index <= 0) {
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

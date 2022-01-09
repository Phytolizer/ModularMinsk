#ifndef PHYTO_HASH_HASH_H_
#define PHYTO_HASH_HASH_H_

#include <phyto/string/string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define PHYTO_HASH_FLAGS_X                        \
    X(ok, "ok")                                   \
    X(error, "error")                             \
    X(duplicate, "duplicate key")                 \
    X(empty, "empty hash")                        \
    X(not_found, "key not found")                 \
    X(overflow, "overflow calculating next size") \
    X(alloc, "allocation failure")

typedef enum {
#define X(x, y) phyto_hash_flag_##x,
    PHYTO_HASH_FLAGS_X
#undef X
} phyto_hash_flag_t;

phyto_string_span_t phyto_hash_flag_explain(phyto_hash_flag_t flag);

typedef PHYTO_SPAN_TYPE(uint64_t) phyto_hash_prime_span_t;

phyto_hash_prime_span_t phyto_hash_prime_span(void);

uint64_t phyto_hash_fnv1a(phyto_string_span_t s);
uint64_t phyto_hash_djb2(phyto_string_span_t s);
extern const double phyto_hash_default_load;

#define PHYTO_HASH_DECL(Name, V)                                                               \
    typedef enum {                                                                             \
        Name##_entry_state_empty,                                                              \
        Name##_entry_state_filled,                                                             \
        Name##_entry_state_deleted,                                                            \
    } Name##_entry_state_t;                                                                    \
    typedef struct {                                                                           \
        phyto_string_t key;                                                                    \
        V value;                                                                               \
        Name##_entry_state_t state;                                                            \
    } Name##_entry_t;                                                                          \
    typedef struct {                                                                           \
        uint64_t (*hash)(phyto_string_span_t);                                                 \
    } Name##_key_ops_t;                                                                        \
    typedef struct {                                                                           \
        int32_t (*compare)(V, V);                                                              \
        V (*copy)(V);                                                                          \
        void (*print)(FILE*, V);                                                               \
        void (*free)(V);                                                                       \
        uint64_t (*hash)(V);                                                                   \
    } Name##_value_ops_t;                                                                      \
    typedef struct {                                                                           \
        Name##_entry_t* buffer;                                                                \
        size_t capacity;                                                                       \
        size_t count;                                                                          \
        double load;                                                                           \
        phyto_hash_flag_t flag;                                                                \
        const Name##_key_ops_t* key_ops;                                                       \
        const Name##_value_ops_t* value_ops;                                                   \
    } Name##_t;                                                                                \
    Name##_t* Name##_new(size_t capacity, double load, const Name##_key_ops_t* key_ops,        \
                         const Name##_value_ops_t* value_ops);                                 \
    void Name##_clear(Name##_t* map);                                                          \
    void Name##_free(Name##_t* map);                                                           \
    bool Name##_insert(Name##_t* map, phyto_string_span_t key, V value);                       \
    bool Name##_update(Name##_t* map, phyto_string_span_t key, V new_value, V* out_old_value); \
    bool Name##_remove(Name##_t* map, phyto_string_span_t key, V* out_value);                  \
    bool Name##_max(Name##_t* map, phyto_string_t* out_key, V* out_value);                     \
    bool Name##_min(Name##_t* map, phyto_string_t* out_key, V* out_value);                     \
    V Name##_get(Name##_t* map, phyto_string_span_t key);                                      \
    V* Name##_get_ref(Name##_t* map, phyto_string_span_t key);                                 \
    bool Name##_contains(Name##_t* map, phyto_string_span_t key);                              \
    bool Name##_empty(Name##_t* map);                                                          \
    bool Name##_full(Name##_t* map);                                                           \
    size_t Name##_count(Name##_t* map);                                                        \
    size_t Name##_capacity(Name##_t* map);                                                     \
    double Name##_load(Name##_t* map);                                                         \
    phyto_hash_flag_t Name##_flag(Name##_t* map);                                              \
    bool Name##_resize(Name##_t* map, size_t new_capacity);                                    \
    Name##_t* Name##_copy_of(Name##_t* map);                                                   \
    bool Name##_equals(Name##_t* map, Name##_t* other)

#define PHYTO_HASH_IMPL(Name, V)                                                                \
    static Name##_entry_t* Name##_impl_get_entry(Name##_t* map, phyto_string_span_t key);       \
    static size_t Name##_impl_calculate_size(size_t required);                                  \
    Name##_t* Name##_new(size_t capacity, double load, const Name##_key_ops_t* key_ops,         \
                         const Name##_value_ops_t* value_ops) {                                 \
        if (capacity == 0 || load <= 0 || load >= 1) {                                          \
            return NULL;                                                                        \
        }                                                                                       \
        if ((double)capacity >= (double)UINTMAX_MAX * load) {                                   \
            return NULL;                                                                        \
        }                                                                                       \
        if (!value_ops) {                                                                       \
            return NULL;                                                                        \
        }                                                                                       \
        size_t real_capacity = Name##_impl_calculate_size((size_t)((double)capacity / load));   \
        Name##_t* map = malloc(sizeof(Name##_t));                                               \
        if (!map) {                                                                             \
            return NULL;                                                                        \
        }                                                                                       \
        map->buffer = calloc(real_capacity, sizeof(Name##_entry_t));                            \
        if (!map->buffer) {                                                                     \
            free(map);                                                                          \
            return NULL;                                                                        \
        }                                                                                       \
        map->count = 0;                                                                         \
        map->capacity = real_capacity;                                                          \
        map->load = load;                                                                       \
        map->flag = phyto_hash_flag_ok;                                                         \
        map->key_ops = key_ops;                                                                 \
        map->value_ops = value_ops;                                                             \
        return map;                                                                             \
    }                                                                                           \
    void Name##_clear(Name##_t* map) {                                                          \
        for (size_t i = 0; i < map->capacity; ++i) {                                            \
            Name##_entry_t* entry = &map->buffer[i];                                            \
            if (entry->state == Name##_entry_state_filled) {                                    \
                phyto_string_free(&entry->key);                                                 \
                if (map->value_ops->free) {                                                     \
                    map->value_ops->free(entry->value);                                         \
                }                                                                               \
            }                                                                                   \
        }                                                                                       \
        memset(map->buffer, 0, sizeof(Name##_entry_t) * map->capacity);                         \
        map->count = 0;                                                                         \
        map->flag = phyto_hash_flag_ok;                                                         \
    }                                                                                           \
    void Name##_free(Name##_t* map) {                                                           \
        for (size_t i = 0; i < map->capacity; ++i) {                                            \
            Name##_entry_t* entry = &map->buffer[i];                                            \
            if (entry->state == Name##_entry_state_filled) {                                    \
                phyto_string_free(&entry->key);                                                 \
                if (map->value_ops->free) {                                                     \
                    map->value_ops->free(entry->value);                                         \
                }                                                                               \
            }                                                                                   \
        }                                                                                       \
        free(map->buffer);                                                                      \
        free(map);                                                                              \
    }                                                                                           \
    bool Name##_insert(Name##_t* map, phyto_string_span_t key, V value) {                       \
        if (Name##_full(map)) {                                                                 \
            if (!Name##_resize(map, map->capacity + 1)) {                                       \
                return false;                                                                   \
            }                                                                                   \
        }                                                                                       \
        if (Name##_impl_get_entry(map, key) != NULL) {                                          \
            map->flag = phyto_hash_flag_duplicate;                                              \
            return false;                                                                       \
        }                                                                                       \
        size_t hash = map->key_ops->hash(key);                                                  \
        size_t original_pos = hash % map->capacity;                                             \
        size_t pos = original_pos;                                                              \
                                                                                                \
        Name##_entry_t* target = &map->buffer[pos];                                             \
        if (target->state == Name##_entry_state_empty ||                                        \
            target->state == Name##_entry_state_deleted) {                                      \
            target->key = phyto_string_own(key);                                                \
            target->value = value;                                                              \
            target->state = Name##_entry_state_filled;                                          \
        } else {                                                                                \
            while (true) {                                                                      \
                pos++;                                                                          \
                target = &map->buffer[pos % map->capacity];                                     \
                if (target->state == Name##_entry_state_empty ||                                \
                    target->state == Name##_entry_state_deleted) {                              \
                    target->key = phyto_string_own(key);                                        \
                    target->value = value;                                                      \
                    target->state = Name##_entry_state_filled;                                  \
                    break;                                                                      \
                }                                                                               \
            }                                                                                   \
        }                                                                                       \
        map->count++;                                                                           \
        map->flag = phyto_hash_flag_ok;                                                         \
        return true;                                                                            \
    }                                                                                           \
    bool Name##_update(Name##_t* map, phyto_string_span_t key, V new_value, V* out_old_value) { \
        if (Name##_empty(map)) {                                                                \
            map->flag = phyto_hash_flag_empty;                                                  \
            return false;                                                                       \
        }                                                                                       \
        Name##_entry_t* entry = Name##_impl_get_entry(map, key);                                \
        if (!entry) {                                                                           \
            map->flag = phyto_hash_flag_not_found;                                              \
            return false;                                                                       \
        }                                                                                       \
        if (out_old_value) {                                                                    \
            *out_old_value = entry->value;                                                      \
        }                                                                                       \
        entry->value = new_value;                                                               \
        map->flag = phyto_hash_flag_ok;                                                         \
        return true;                                                                            \
    }                                                                                           \
    bool Name##_remove(Name##_t* map, phyto_string_span_t key, V* out_value) {                  \
        if (Name##_empty(map)) {                                                                \
            map->flag = phyto_hash_flag_empty;                                                  \
            return false;                                                                       \
        }                                                                                       \
        Name##_entry_t* entry = Name##_impl_get_entry(map, key);                                \
        if (!entry) {                                                                           \
            map->flag = phyto_hash_flag_not_found;                                              \
            return false;                                                                       \
        }                                                                                       \
        if (out_value) {                                                                        \
            *out_value = entry->value;                                                          \
        }                                                                                       \
        phyto_string_free(&entry->key);                                                         \
        entry->value = (V){0};                                                                  \
        entry->state = Name##_entry_state_deleted;                                              \
        map->count--;                                                                           \
        map->flag = phyto_hash_flag_ok;                                                         \
        return true;                                                                            \
    }                                                                                           \
    bool Name##_max(Name##_t* map, phyto_string_t* out_key, V* out_value) {                     \
        if (Name##_empty(map)) {                                                                \
            map->flag = phyto_hash_flag_empty;                                                  \
            return false;                                                                       \
        }                                                                                       \
        bool first = true;                                                                      \
        phyto_string_t max_key = {0};                                                           \
        V max_value = (V){0};                                                                   \
        for (size_t i = 0; i < map->capacity; ++i) {                                            \
            Name##_entry_t* entry = &map->buffer[i];                                            \
            if (entry->state == Name##_entry_state_filled) {                                    \
                if (first) {                                                                    \
                    first = false;                                                              \
                    max_key = entry->key;                                                       \
                    max_value = entry->value;                                                   \
                } else if (phyto_string_compare(phyto_string_as_span(entry->key),               \
                                                phyto_string_as_span(max_key)) > 0) {           \
                    max_key = entry->key;                                                       \
                    max_value = entry->value;                                                   \
                }                                                                               \
            }                                                                                   \
        }                                                                                       \
        if (out_key) {                                                                          \
            *out_key = max_key;                                                                 \
        }                                                                                       \
        if (out_value) {                                                                        \
            *out_value = max_value;                                                             \
        }                                                                                       \
        map->flag = phyto_hash_flag_ok;                                                         \
        return true;                                                                            \
    }                                                                                           \
    bool Name##_min(Name##_t* map, phyto_string_t* out_key, V* out_value) {                     \
        if (Name##_empty(map)) {                                                                \
            map->flag = phyto_hash_flag_empty;                                                  \
            return false;                                                                       \
        }                                                                                       \
        bool first = true;                                                                      \
        phyto_string_t min_key = {0};                                                           \
        V min_value = (V){0};                                                                   \
        for (size_t i = 0; i < map->capacity; ++i) {                                            \
            Name##_entry_t* entry = &map->buffer[i];                                            \
            if (entry->state == Name##_entry_state_filled) {                                    \
                if (first) {                                                                    \
                    first = false;                                                              \
                    min_key = entry->key;                                                       \
                    min_value = entry->value;                                                   \
                } else if (phyto_string_compare(phyto_string_as_span(entry->key),               \
                                                phyto_string_as_span(min_key)) < 0) {           \
                    min_key = entry->key;                                                       \
                    min_value = entry->value;                                                   \
                }                                                                               \
            }                                                                                   \
        }                                                                                       \
        if (out_key) {                                                                          \
            *out_key = min_key;                                                                 \
        }                                                                                       \
        if (out_value) {                                                                        \
            *out_value = min_value;                                                             \
        }                                                                                       \
        map->flag = phyto_hash_flag_ok;                                                         \
        return true;                                                                            \
    }                                                                                           \
    V Name##_get(Name##_t* map, phyto_string_span_t key) {                                      \
        if (Name##_empty(map)) {                                                                \
            map->flag = phyto_hash_flag_empty;                                                  \
            return (V){0};                                                                      \
        }                                                                                       \
        Name##_entry_t* entry = Name##_impl_get_entry(map, key);                                \
        if (!entry) {                                                                           \
            map->flag = phyto_hash_flag_not_found;                                              \
            return (V){0};                                                                      \
        }                                                                                       \
        map->flag = phyto_hash_flag_ok;                                                         \
        return entry->value;                                                                    \
    }                                                                                           \
    V* Name##_get_ref(Name##_t* map, phyto_string_span_t key) {                                 \
        if (Name##_empty(map)) {                                                                \
            map->flag = phyto_hash_flag_empty;                                                  \
            return NULL;                                                                        \
        }                                                                                       \
        Name##_entry_t* entry = Name##_impl_get_entry(map, key);                                \
        if (!entry) {                                                                           \
            map->flag = phyto_hash_flag_not_found;                                              \
            return NULL;                                                                        \
        }                                                                                       \
        map->flag = phyto_hash_flag_ok;                                                         \
        return &entry->value;                                                                   \
    }                                                                                           \
    bool Name##_contains(Name##_t* map, phyto_string_span_t key) {                              \
        map->flag = phyto_hash_flag_ok;                                                         \
        return Name##_impl_get_entry(map, key) != NULL;                                         \
    }                                                                                           \
    bool Name##_empty(Name##_t* map) { return map->count == 0; }                                \
    bool Name##_full(Name##_t* map) {                                                           \
        return (double)map->capacity * map->load <= (double)map->count;                         \
    }                                                                                           \
    size_t Name##_count(Name##_t* map) { return map->count; }                                   \
    size_t Name##_capacity(Name##_t* map) { return map->capacity; }                             \
    double Name##_load(Name##_t* map) { return map->load; }                                     \
    phyto_hash_flag_t Name##_flag(Name##_t* map) { return map->flag; }                          \
    bool Name##_resize(Name##_t* map, size_t capacity) {                                        \
        map->flag = phyto_hash_flag_ok;                                                         \
        if (capacity == map->capacity) {                                                        \
            return true;                                                                        \
        }                                                                                       \
        if ((double)map->capacity > (double)capacity / map->load) {                             \
            return true;                                                                        \
        }                                                                                       \
        if ((double)capacity >= (double)UINTMAX_MAX * map->load) {                              \
            map->flag = phyto_hash_flag_error;                                                  \
            return false;                                                                       \
        }                                                                                       \
        size_t theoretical_size = Name##_impl_calculate_size(capacity);                         \
        if ((double)theoretical_size < (double)map->count / map->load) {                        \
            map->flag = phyto_hash_flag_overflow;                                               \
            return false;                                                                       \
        }                                                                                       \
        Name##_t* new_map = Name##_new(capacity, map->load, map->key_ops, map->value_ops);      \
        if (!new_map) {                                                                         \
            map->flag = phyto_hash_flag_alloc;                                                  \
            return false;                                                                       \
        }                                                                                       \
        for (size_t i = 0; i < map->capacity; ++i) {                                            \
            if (map->buffer[i].state == Name##_entry_state_filled) {                            \
                phyto_string_span_t key = phyto_string_as_span(map->buffer[i].key);             \
                V value = map->buffer[i].value;                                                 \
                if (!Name##_insert(new_map, key, value)) {                                      \
                    map->flag = new_map->flag;                                                  \
                    Name##_free(new_map);                                                       \
                    return false;                                                               \
                }                                                                               \
            }                                                                                   \
        }                                                                                       \
        if (map->count != new_map->count) {                                                     \
            map->flag = phyto_hash_flag_error;                                                  \
            Name##_free(new_map);                                                               \
            return false;                                                                       \
        }                                                                                       \
        Name##_entry_t* tmp_b = map->buffer;                                                    \
        map->buffer = new_map->buffer;                                                          \
        new_map->buffer = tmp_b;                                                                \
        size_t tmp_c = map->capacity;                                                           \
        map->capacity = new_map->capacity;                                                      \
        new_map->capacity = tmp_c;                                                              \
        new_map->value_ops = &((Name##_value_ops_t){0});                                        \
        Name##_free(new_map);                                                                   \
        return true;                                                                            \
    }                                                                                           \
    Name##_t* Name##_copy_of(Name##_t* map) {                                                   \
        Name##_t* result = Name##_new(map->capacity, map->load, map->key_ops, map->value_ops);  \
        if (!result) {                                                                          \
            map->flag = phyto_hash_flag_error;                                                  \
            return NULL;                                                                        \
        }                                                                                       \
        for (size_t i = 0; i < map->capacity; ++i) {                                            \
            Name##_entry_t* scan = &map->buffer[i];                                             \
            if (scan->state != Name##_entry_state_empty) {                                      \
                Name##_entry_t* target = &result->buffer[i];                                    \
                if (scan->state == Name##_entry_state_deleted) {                                \
                    target->state = Name##_entry_state_deleted;                                 \
                } else {                                                                        \
                    target->state = Name##_entry_state_filled;                                  \
                    target->key = phyto_string_copy(scan->key);                                 \
                    if (map->value_ops->copy) {                                                 \
                        target->value = map->value_ops->copy(scan->value);                      \
                    } else {                                                                    \
                        target->value = scan->value;                                            \
                    }                                                                           \
                }                                                                               \
            }                                                                                   \
        }                                                                                       \
        result->count = map->count;                                                             \
        map->flag = phyto_hash_flag_ok;                                                         \
        return result;                                                                          \
    }                                                                                           \
    bool Name##_equals(Name##_t* map1, Name##_t* map2) {                                        \
        map1->flag = phyto_hash_flag_ok;                                                        \
        map2->flag = phyto_hash_flag_ok;                                                        \
        if (map1->count != map2->count) {                                                       \
            return false;                                                                       \
        }                                                                                       \
        Name##_t* map_a = map1->capacity < map2->capacity ? map1 : map2;                        \
        Name##_t* map_b = map_a == map1 ? map2 : map1;                                          \
        for (size_t i = 0; i < map_a->capacity; ++i) {                                          \
            if (map_a->buffer[i].state == Name##_entry_state_filled) {                          \
                Name##_entry_t* entry_a = &map_a->buffer[i];                                    \
                Name##_entry_t* entry_b =                                                       \
                    Name##_impl_get_entry(map_b, phyto_string_as_span(entry_a->key));           \
                if (!entry_b) {                                                                 \
                    return false;                                                               \
                }                                                                               \
                if (map_a->value_ops->compare(entry_a->value, entry_b->value) != 0) {           \
                    return false;                                                               \
                }                                                                               \
            }                                                                                   \
        }                                                                                       \
        return true;                                                                            \
    }                                                                                           \
    static Name##_entry_t* Name##_impl_get_entry(Name##_t* map, phyto_string_span_t key) {      \
        size_t hash = map->key_ops->hash(key);                                                  \
        size_t pos = hash % map->capacity;                                                      \
        Name##_entry_t* target = &map->buffer[pos];                                             \
        while (target->state == Name##_entry_state_filled ||                                    \
               target->state == Name##_entry_state_deleted) {                                   \
            if (phyto_string_span_equal(key, phyto_string_as_span(target->key))) {              \
                return target;                                                                  \
            }                                                                                   \
            pos++;                                                                              \
            target = &map->buffer[pos % map->capacity];                                         \
        }                                                                                       \
        return NULL;                                                                            \
    }                                                                                           \
    static size_t Name##_impl_calculate_size(size_t required) {                                 \
        const phyto_hash_prime_span_t prime_span = phyto_hash_prime_span();                     \
        const size_t count = prime_span.size;                                                   \
        if (prime_span.begin[count - 1] < required) {                                           \
            return required;                                                                    \
        }                                                                                       \
        size_t i = 0;                                                                           \
        while (prime_span.begin[i] < required) {                                                \
            i++;                                                                                \
        }                                                                                       \
        return prime_span.begin[i];                                                             \
    }

#define PHYTO_HASH_DECL_ITER(Name, V)                            \
    typedef struct {                                             \
        Name##_t* target;                                        \
        size_t cursor;                                           \
        size_t index;                                            \
        size_t first;                                            \
        size_t last;                                             \
        bool at_start;                                           \
        bool at_end;                                             \
    } Name##_iter_t;                                             \
    Name##_iter_t Name##_iter_start(Name##_t* target);           \
    Name##_iter_t Name##_iter_end(Name##_t* target);             \
    bool Name##_iter_at_start(Name##_iter_t* iter);              \
    bool Name##_iter_at_end(Name##_iter_t* iter);                \
    bool Name##_iter_to_start(Name##_iter_t* iter);              \
    bool Name##_iter_to_end(Name##_iter_t* iter);                \
    bool Name##_iter_next(Name##_iter_t* iter);                  \
    bool Name##_iter_prev(Name##_iter_t* iter);                  \
    bool Name##_iter_advance(Name##_iter_t* iter, size_t steps); \
    bool Name##_iter_rewind(Name##_iter_t* iter, size_t steps);  \
    bool Name##_iter_go_to(Name##_iter_t* iter, size_t index);   \
    phyto_string_span_t Name##_iter_key(Name##_iter_t* iter);    \
    V Name##_iter_value(Name##_iter_t* iter);                    \
    V* Name##_iter_value_ref(Name##_iter_t* iter);               \
    size_t Name##_iter_index(Name##_iter_t* iter)

#define PHYTO_HASH_IMPL_ITER(Name, V)                                           \
    Name##_iter_t Name##_iter_start(Name##_t* target) {                         \
        Name##_iter_t iter = {                                                  \
            .target = target,                                                   \
            .cursor = 0,                                                        \
            .index = 0,                                                         \
            .first = 0,                                                         \
            .last = 0,                                                          \
            .at_start = true,                                                   \
            .at_end = Name##_empty(target),                                     \
        };                                                                      \
        if (!Name##_empty(target)) {                                            \
            for (size_t i = 0; i < target->capacity; ++i) {                     \
                if (target->buffer[i].state == Name##_entry_state_filled) {     \
                    iter.first = i;                                             \
                    break;                                                      \
                }                                                               \
            }                                                                   \
            iter.cursor = iter.first;                                           \
            for (size_t i = target->capacity; i > 0; --i) {                     \
                if (target->buffer[i - 1].state == Name##_entry_state_filled) { \
                    iter.last = i - 1;                                          \
                    break;                                                      \
                }                                                               \
            }                                                                   \
        }                                                                       \
        return iter;                                                            \
    }                                                                           \
    Name##_iter_t Name##_iter_end(Name##_t* target) {                           \
        Name##_iter_t iter = {                                                  \
            .target = target,                                                   \
            .cursor = 0,                                                        \
            .index = 0,                                                         \
            .first = 0,                                                         \
            .last = 0,                                                          \
            .at_start = Name##_empty(target),                                   \
            .at_end = true,                                                     \
        };                                                                      \
        if (!Name##_empty(target)) {                                            \
            for (size_t i = 0; i < target->capacity; ++i) {                     \
                if (target->buffer[i].state == Name##_entry_state_filled) {     \
                    iter.first = i;                                             \
                    break;                                                      \
                }                                                               \
            }                                                                   \
            for (size_t i = target->capacity; i > 0; --i) {                     \
                if (target->buffer[i - 1].state == Name##_entry_state_filled) { \
                    iter.last = i - 1;                                          \
                    break;                                                      \
                }                                                               \
            }                                                                   \
            iter.cursor = iter.last;                                            \
            iter.index = target->count - 1;                                     \
        }                                                                       \
        return iter;                                                            \
    }                                                                           \
    bool Name##_iter_at_start(Name##_iter_t* iter) {                            \
        return Name##_empty(iter->target) || iter->at_start;                    \
    }                                                                           \
    bool Name##_iter_at_end(Name##_iter_t* iter) {                              \
        return Name##_empty(iter->target) || iter->at_end;                      \
    }                                                                           \
    bool Name##_iter_to_start(Name##_iter_t* iter) {                            \
        if (!Name##_empty(iter->target)) {                                      \
            iter->cursor = iter->first;                                         \
            iter->index = 0;                                                    \
            iter->at_start = true;                                              \
            iter->at_end = false;                                               \
            return true;                                                        \
        }                                                                       \
        return false;                                                           \
    }                                                                           \
    bool Name##_iter_to_end(Name##_iter_t* iter) {                              \
        if (!Name##_empty(iter->target)) {                                      \
            iter->cursor = iter->last;                                          \
            iter->index = iter->target->count - 1;                              \
            iter->at_start = false;                                             \
            iter->at_end = true;                                                \
            return true;                                                        \
        }                                                                       \
        return false;                                                           \
    }                                                                           \
    bool Name##_iter_next(Name##_iter_t* iter) {                                \
        if (iter->at_end) {                                                     \
            return false;                                                       \
        }                                                                       \
        if (iter->index + 1 == iter->target->count) {                           \
            iter->at_end = true;                                                \
            return false;                                                       \
        }                                                                       \
        iter->at_start = Name##_empty(iter->target);                            \
        Name##_entry_t* scan = &iter->target->buffer[iter->cursor];             \
        iter->index++;                                                          \
        while (true) {                                                          \
            iter->cursor++;                                                     \
            scan = &iter->target->buffer[iter->cursor];                         \
            if (scan->state == Name##_entry_state_filled) {                     \
                break;                                                          \
            }                                                                   \
        }                                                                       \
        return true;                                                            \
    }                                                                           \
    bool Name##_iter_prev(Name##_iter_t* iter) {                                \
        if (iter->at_start) {                                                   \
            return false;                                                       \
        }                                                                       \
        if (iter->index == 0) {                                                 \
            iter->at_start = true;                                              \
            return false;                                                       \
        }                                                                       \
        iter->at_end = Name##_empty(iter->target);                              \
        Name##_entry_t* scan = &iter->target->buffer[iter->cursor];             \
        iter->index--;                                                          \
        while (true) {                                                          \
            iter->cursor--;                                                     \
            scan = &iter->target->buffer[iter->cursor];                         \
            if (scan->state == Name##_entry_state_filled) {                     \
                break;                                                          \
            }                                                                   \
        }                                                                       \
        return true;                                                            \
    }                                                                           \
    bool Name##_iter_advance(Name##_iter_t* iter, size_t steps) {               \
        if (iter->at_end) {                                                     \
            return false;                                                       \
        }                                                                       \
        if (iter->index + 1 == iter->target->count) {                           \
            iter->at_end = true;                                                \
            return false;                                                       \
        }                                                                       \
        if (steps == 0 || iter->index + steps >= iter->target->count) {         \
            return false;                                                       \
        }                                                                       \
        for (size_t i = 0; i < steps; ++i) {                                    \
            Name##_iter_next(iter);                                             \
        }                                                                       \
        return true;                                                            \
    }                                                                           \
    bool Name##_iter_rewind(Name##_iter_t* iter, size_t steps) {                \
        if (iter->at_start) {                                                   \
            return false;                                                       \
        }                                                                       \
        if (iter->index == 0) {                                                 \
            iter->at_start = true;                                              \
            return false;                                                       \
        }                                                                       \
        if (steps == 0 || iter->index < steps) {                                \
            return false;                                                       \
        }                                                                       \
        for (size_t i = 0; i < steps; ++i) {                                    \
            Name##_iter_prev(iter);                                             \
        }                                                                       \
        return true;                                                            \
    }                                                                           \
    bool Name##_iter_go_to(Name##_iter_t* iter, size_t index) {                 \
        if (index >= iter->target->count) {                                     \
            return false;                                                       \
        }                                                                       \
        if (iter->index > index) {                                              \
            return Name##_iter_rewind(iter, iter->index - index);               \
        }                                                                       \
        if (iter->index < index) {                                              \
            return Name##_iter_advance(iter, index - iter->index);              \
        }                                                                       \
        return true;                                                            \
    }                                                                           \
    phyto_string_span_t Name##_iter_key(Name##_iter_t* iter) {                  \
        if (Name##_empty(iter->target)) {                                       \
            return phyto_string_span_empty();                                   \
        }                                                                       \
        return phyto_string_as_span(iter->target->buffer[iter->cursor].key);    \
    }                                                                           \
    V Name##_iter_value(Name##_iter_t* iter) {                                  \
        if (Name##_empty(iter->target)) {                                       \
            return (V){0};                                                      \
        }                                                                       \
        return iter->target->buffer[iter->cursor].value;                        \
    }                                                                           \
    V* Name##_iter_value_ref(Name##_iter_t* iter) {                             \
        if (Name##_empty(iter->target)) {                                       \
            return NULL;                                                        \
        }                                                                       \
        return &iter->target->buffer[iter->cursor].value;                       \
    }                                                                           \
    size_t Name##_iter_index(Name##_iter_t* iter) { return iter->index; }

#define PHYTO_HASH_DECL_STR(Name, V)                                                        \
    void Name##_string(Name##_t* map, void (*cb)(const char* str, size_t len, void* state), \
                       void* state);                                                        \
    void Name##_print(Name##_t* map, FILE* fp, phyto_string_span_t start,                   \
                      phyto_string_span_t separator, phyto_string_span_t end,               \
                      phyto_string_span_t key_value_separator)

#define PHYTO_HASH_IMPL_STR(Name, V)                                                        \
    void Name##_string(Name##_t* map, void (*cb)(const char* str, size_t len, void* state), \
                       void* state) {                                                       \
        char buf[32];                                                                       \
        cb(#Name, sizeof(#Name) - 1, state);                                                \
        cb("_t<phyto_string_t", 17, state);                                                 \
        cb(", ", 2, state);                                                                 \
        cb(#V, sizeof(#V) - 1, state);                                                      \
        cb("> at ", 5, state);                                                              \
        snprintf(buf, sizeof(buf), "%p", map);                                              \
        cb(buf, strlen(buf), state);                                                        \
        cb(" { buffer:", 10, state);                                                        \
        snprintf(buf, sizeof(buf), "%p", map->buffer);                                      \
        cb(buf, strlen(buf), state);                                                        \
        cb(", capacity:", 11, state);                                                       \
        snprintf(buf, sizeof(buf), "%" PRIuMAX, (uintmax_t)map->capacity);                  \
        cb(buf, strlen(buf), state);                                                        \
        cb(", count:", 8, state);                                                           \
        snprintf(buf, sizeof(buf), "%" PRIuMAX, (uintmax_t)map->count);                     \
        cb(buf, strlen(buf), state);                                                        \
        cb(", load:", 7, state);                                                            \
        snprintf(buf, sizeof(buf), "%lf", map->load);                                       \
        cb(buf, strlen(buf), state);                                                        \
        cb(", flag:", 7, state);                                                            \
        phyto_string_span_t flag = phyto_hash_flag_explain(map->flag);                      \
        cb(flag.begin, flag.size, state);                                                   \
        cb(", key_ops:", 10, state);                                                        \
        snprintf(buf, sizeof(buf), "%p", map->key_ops);                                     \
        cb(buf, strlen(buf), state);                                                        \
        cb(", value_ops:", 12, state);                                                      \
        snprintf(buf, sizeof(buf), "%p", map->value_ops);                                   \
        cb(buf, strlen(buf), state);                                                        \
        cb(" }", 2, state);                                                                 \
    }                                                                                       \
    void Name##_print(Name##_t* map, FILE* fp, phyto_string_span_t start,                   \
                      phyto_string_span_t separator, phyto_string_span_t end,               \
                      phyto_string_span_t key_value_separator) {                            \
        fprintf(fp, "%" PHYTO_STRING_FORMAT, PHYTO_STRING_VIEW_PRINTF_ARGS(start));         \
        size_t last = 0;                                                                    \
        for (size_t i = map->capacity; i > 0; --i) {                                        \
            if (map->buffer[i - 1].state == Name##_entry_state_filled) {                    \
                last = i - 1;                                                               \
                break;                                                                      \
            }                                                                               \
        }                                                                                   \
        for (size_t i = 0; i < map->capacity; ++i) {                                        \
            Name##_entry_t* entry = &map->buffer[i];                                        \
            if (entry->state == Name##_entry_state_filled) {                                \
                phyto_string_span_print_to(phyto_string_as_span(entry->key), fp);           \
                fprintf(fp, "%" PHYTO_STRING_FORMAT,                                        \
                        PHYTO_STRING_VIEW_PRINTF_ARGS(key_value_separator));                \
                map->value_ops->print(fp, entry->value);                                    \
                if (i < last) {                                                             \
                    fprintf(fp, "%" PHYTO_STRING_FORMAT,                                    \
                            PHYTO_STRING_VIEW_PRINTF_ARGS(separator));                      \
                }                                                                           \
            }                                                                               \
        }                                                                                   \
        fprintf(fp, "%" PHYTO_STRING_FORMAT, PHYTO_STRING_VIEW_PRINTF_ARGS(end));           \
    }

#endif  // PHYTO_HASH_HASH_H_

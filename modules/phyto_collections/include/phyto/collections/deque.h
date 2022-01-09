#ifndef PHYTO_DEQUE_DEQUE_H_
#define PHYTO_DEQUE_DEQUE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "phyto/collections/callbacks.h"

#define PHYTO_COLLECTIONS_DEQUE_ERROR_FLAGS_X \
    X(ok, "ok")                               \
    X(alloc, "allocation error")              \
    X(empty, "empty deque")                   \
    X(not_found, "not found")                 \
    X(invalid, "invalid argument")            \
    X(range, "out of range")                  \
    X(duplicate, "duplicate")                 \
    X(error, "generic error")                 \
    X(thread, "threading error")              \
    X(mutex, "mutex error")                   \
    X(full, "full deque")

typedef enum {
#define X(x, y) phyto_deque_error_flag_##x,
    PHYTO_COLLECTIONS_DEQUE_ERROR_FLAGS_X
#undef X
} phyto_deque_error_flag_t;

#define PHYTO_COLLECTIONS_DEQUE_DECL(Name, V)                                                     \
    typedef struct {                                                                              \
        PHYTO_COLLECTIONS_FREE_CB(V);                                                             \
        PHYTO_COLLECTIONS_COPY_CB(V);                                                             \
        PHYTO_COLLECTIONS_COMPARE_CB(V);                                                          \
        PHYTO_COLLECTIONS_PRINT_CB(V);                                                            \
    } Name##_callbacks_t;                                                                         \
    typedef struct {                                                                              \
        V* data;                                                                                  \
        size_t capacity;                                                                          \
        size_t count;                                                                             \
        size_t front;                                                                             \
        size_t back;                                                                              \
        phyto_deque_error_flag_t error_flag;                                                      \
        const Name##_callbacks_t* callbacks;                                                      \
    } Name##_t;                                                                                   \
                                                                                                  \
    Name##_t Name##_new(size_t capacity, const Name##_callbacks_t* callbacks);                    \
    void Name##_clear(Name##_t* deque);                                                           \
    void Name##_free(Name##_t* deque);                                                            \
    bool Name##_push_front(Name##_t* deque, V value);                                             \
    bool Name##_push_back(Name##_t* deque, V value);                                              \
    bool Name##_pop_front(Name##_t* deque);                                                       \
    bool Name##_pop_back(Name##_t* deque);                                                        \
    V Name##_front(Name##_t* deque);                                                              \
    V Name##_back(Name##_t* deque);                                                               \
    bool Name##_contains(Name##_t* deque, V value);                                               \
    bool Name##_empty(Name##_t* deque);                                                           \
    bool Name##_full(Name##_t* deque);                                                            \
    size_t Name##_count(Name##_t* deque);                                                         \
    size_t Name##_capacity(Name##_t* deque);                                                      \
    phyto_deque_error_flag_t Name##_error_flag(Name##_t deque);                                   \
    bool Name##_resize(Name##_t* deque, size_t capacity);                                         \
    Name##_t Name##_copy_of(Name##_t deque);                                                      \
    bool Name##_equals(Name##_t a, Name##_t b);                                                   \
    void Name##_string(Name##_t deque, void (*string_cb)(char* data, size_t size, void* context), \
                       void* context);                                                            \
    void Name##_print(Name##_t deque, FILE* stream, const char* sep);

#define PHYTO_COLLECTIONS_DEQUE_IMPL(Name, V)                                                     \
    Name##_t Name##_new(size_t capacity, const Name##_callbacks_t* callbacks) {                   \
        if (capacity < 1) {                                                                       \
            return (Name##_t){.error_flag = phyto_deque_error_flag_invalid};                      \
        }                                                                                         \
        if (callbacks == NULL) {                                                                  \
            return (Name##_t){.error_flag = phyto_deque_error_flag_invalid};                      \
        }                                                                                         \
        Name##_t deque;                                                                           \
        deque.data = calloc(capacity, sizeof(V));                                                 \
        if (!deque.data) {                                                                        \
            deque.error_flag = phyto_deque_error_flag_alloc;                                      \
            return deque;                                                                         \
        }                                                                                         \
        deque.capacity = capacity;                                                                \
        deque.count = 0;                                                                          \
        deque.front = 0;                                                                          \
        deque.back = 0;                                                                           \
        deque.error_flag = phyto_deque_error_flag_ok;                                             \
        deque.callbacks = callbacks;                                                              \
        return deque;                                                                             \
    }                                                                                             \
    void Name##_clear(Name##_t* deque) {                                                          \
        if (deque->callbacks->free_cb) {                                                          \
            for (size_t i = deque->front, j = 0; j < deque->count; ++j) {                         \
                deque->callbacks->free_cb(&deque->data[i]);                                       \
                i = (i + 1) % deque->capacity;                                                    \
            }                                                                                     \
        }                                                                                         \
        deque->count = 0;                                                                         \
        deque->front = 0;                                                                         \
        deque->back = 0;                                                                          \
        deque->error_flag = phyto_deque_error_flag_ok;                                            \
    }                                                                                             \
    void Name##_free(Name##_t* deque) {                                                           \
        if (deque->callbacks->free_cb) {                                                          \
            for (size_t i = deque->front, j = 0; j < deque->count; ++j) {                         \
                deque->callbacks->free_cb(&deque->data[i]);                                       \
                i = (i + 1) % deque->capacity;                                                    \
            }                                                                                     \
        }                                                                                         \
        free(deque->data);                                                                        \
        deque->data = NULL;                                                                       \
        deque->capacity = 0;                                                                      \
        deque->count = 0;                                                                         \
        deque->front = 0;                                                                         \
        deque->back = 0;                                                                          \
        deque->error_flag = phyto_deque_error_flag_ok;                                            \
    }                                                                                             \
    bool Name##_push_front(Name##_t* deque, V value) {                                            \
        if (Name##_full(deque)) {                                                                 \
            if (!Name##_resize(deque, deque->capacity * 2)) {                                     \
                return false;                                                                     \
            }                                                                                     \
        }                                                                                         \
        deque->front = (deque->front == 0) ? deque->capacity - 1 : deque->front - 1;              \
        deque->data[deque->front] = value;                                                        \
        ++deque->count;                                                                           \
        deque->error_flag = phyto_deque_error_flag_ok;                                            \
        return true;                                                                              \
    }                                                                                             \
    bool Name##_push_back(Name##_t* deque, V value) {                                             \
        if (Name##_full(deque)) {                                                                 \
            if (!Name##_resize(deque, deque->capacity * 2)) {                                     \
                return false;                                                                     \
            }                                                                                     \
        }                                                                                         \
        deque->data[deque->back] = value;                                                         \
        deque->back = (deque->back + 1) % deque->capacity;                                        \
        ++deque->count;                                                                           \
        deque->error_flag = phyto_deque_error_flag_ok;                                            \
        return true;                                                                              \
    }                                                                                             \
    bool Name##_pop_front(Name##_t* deque) {                                                      \
        if (Name##_empty(deque)) {                                                                \
            deque->error_flag = phyto_deque_error_flag_empty;                                     \
            return false;                                                                         \
        }                                                                                         \
        deque->data[deque->front] = (V){0};                                                       \
        deque->front = (deque->front + 1) % deque->capacity;                                      \
        --deque->count;                                                                           \
        deque->error_flag = phyto_deque_error_flag_ok;                                            \
        return true;                                                                              \
    }                                                                                             \
    bool Name##_pop_back(Name##_t* deque) {                                                       \
        if (Name##_empty(deque)) {                                                                \
            deque->error_flag = phyto_deque_error_flag_empty;                                     \
            return false;                                                                         \
        }                                                                                         \
        deque->back = (deque->back == 0) ? deque->capacity - 1 : deque->back - 1;                 \
        deque->data[deque->back] = (V){0};                                                        \
        --deque->count;                                                                           \
        deque->error_flag = phyto_deque_error_flag_ok;                                            \
        return true;                                                                              \
    }                                                                                             \
    V Name##_front(Name##_t* deque) {                                                             \
        if (Name##_empty(deque)) {                                                                \
            deque->error_flag = phyto_deque_error_flag_empty;                                     \
            return (V){0};                                                                        \
        }                                                                                         \
        deque->error_flag = phyto_deque_error_flag_ok;                                            \
        return deque->data[deque->front];                                                         \
    }                                                                                             \
    V Name##_back(Name##_t* deque) {                                                              \
        if (Name##_empty(deque)) {                                                                \
            deque->error_flag = phyto_deque_error_flag_empty;                                     \
            return (V){0};                                                                        \
        }                                                                                         \
        deque->error_flag = phyto_deque_error_flag_ok;                                            \
        return deque->data[deque->back - 1];                                                      \
    }                                                                                             \
    bool Name##_contains(Name##_t* deque, V value) {                                              \
        bool result = false;                                                                      \
        for (size_t i = deque->front, j = 0; j < deque->count; ++j) {                             \
            if (deque->callbacks->compare_cb(deque->data[i], value)) {                            \
                result = true;                                                                    \
                break;                                                                            \
            }                                                                                     \
            i = (i + 1) % deque->capacity;                                                        \
        }                                                                                         \
        return result;                                                                            \
    }                                                                                             \
    bool Name##_empty(Name##_t* deque) { return deque->count == 0; }                              \
    bool Name##_full(Name##_t* deque) { return deque->count >= deque->capacity; }                 \
    size_t Name##_count(Name##_t* deque) { return deque->count; }                                 \
    size_t Name##_capacity(Name##_t* deque) { return deque->capacity; }                           \
    phyto_deque_error_flag_t Name##_error_flag(Name##_t deque) { return deque.error_flag; }       \
    bool Name##_resize(Name##_t* deque, size_t capacity) {                                        \
        deque->error_flag = phyto_deque_error_flag_ok;                                            \
        if (capacity < deque->count) {                                                            \
            return true;                                                                          \
        }                                                                                         \
        V* new_data = malloc(sizeof(V) * capacity);                                               \
        if (new_data == NULL) {                                                                   \
            deque->error_flag = phyto_deque_error_flag_alloc;                                     \
            return false;                                                                         \
        }                                                                                         \
        for (size_t i = deque->front, j = 0; j < deque->count; ++j) {                             \
            new_data[j] = deque->data[i];                                                         \
            i = (i + 1) % deque->capacity;                                                        \
        }                                                                                         \
        free(deque->data);                                                                        \
        deque->data = new_data;                                                                   \
        deque->capacity = capacity;                                                               \
        deque->front = 0;                                                                         \
        deque->back = deque->count;                                                               \
        return true;                                                                              \
    }                                                                                             \
    Name##_t Name##_copy_of(Name##_t deque) {                                                     \
        Name##_t result = Name##_new(deque.capacity, deque.callbacks);                            \
        if (deque.callbacks->copy_cb) {                                                           \
            for (size_t i = deque.front, j = 0; j < deque.count; ++j) {                           \
                result.data[j] = deque.callbacks->copy_cb(deque.data[i]);                         \
                i = (i + 1) % deque.capacity;                                                     \
            }                                                                                     \
        } else {                                                                                  \
            for (size_t i = deque.front, j = 0; j < deque.count; ++j) {                           \
                result.data[j] = deque.data[i];                                                   \
                i = (i + 1) % deque.capacity;                                                     \
            }                                                                                     \
        }                                                                                         \
        result.count = deque.count;                                                               \
        result.front = 0;                                                                         \
        result.back = deque.count;                                                                \
        return result;                                                                            \
    }                                                                                             \
    bool Name##_equals(Name##_t deque1, Name##_t deque2) {                                        \
        if (deque1.count != deque2.count) {                                                       \
            return false;                                                                         \
        }                                                                                         \
        if (deque1.callbacks->compare_cb != deque2.callbacks->compare_cb) {                       \
            return false;                                                                         \
        }                                                                                         \
        for (size_t i = deque1.front, j = deque2.front, k = 0; k < deque1.count; ++k) {           \
            if (deque1.callbacks->compare_cb(deque1.data[i], deque2.data[i]) != 0) {              \
                return false;                                                                     \
            }                                                                                     \
            i = (i + 1) % deque1.capacity;                                                        \
            j = (j + 1) % deque2.capacity;                                                        \
        }                                                                                         \
        return true;                                                                              \
    }                                                                                             \
    void Name##_string(Name##_t deque, void (*string_cb)(char* data, size_t size, void* context), \
                       void* context) {                                                           \
        string_cb(#Name, sizeof(#Name) - 1, context);                                             \
        string_cb("<", 1, context);                                                               \
        string_cb(#V, sizeof(#V) - 1, context);                                                   \
        string_cb("> at ", 5, context);                                                           \
        char buf[32];                                                                             \
        sprintf(buf, "%p", &deque);                                                               \
        string_cb(buf, strlen(buf), context);                                                     \
        string_cb(" { ", 3, context);                                                             \
        string_cb("buffer:", 7, context);                                                         \
        snprintf(buf, sizeof(buf), "%p", deque.data);                                             \
        string_cb(buf, strlen(buf), context);                                                     \
        string_cb(", capacity:", 11, context);                                                    \
        snprintf(buf, sizeof(buf), "%zu", deque.capacity);                                        \
        string_cb(buf, strlen(buf), context);                                                     \
        string_cb(", count:", 8, context);                                                        \
        snprintf(buf, sizeof(buf), "%zu", deque.count);                                           \
        string_cb(buf, strlen(buf), context);                                                     \
        string_cb(", front:", 9, context);                                                        \
        snprintf(buf, sizeof(buf), "%zu", deque.front);                                           \
        string_cb(buf, strlen(buf), context);                                                     \
        string_cb(", back:", 8, context);                                                         \
        snprintf(buf, sizeof(buf), "%zu", deque.back);                                            \
        string_cb(buf, strlen(buf), context);                                                     \
        string_cb(", error_flag:", 13, context);                                                  \
        snprintf(buf, sizeof(buf), "%d", deque.error_flag);                                       \
        string_cb(buf, strlen(buf), context);                                                     \
        string_cb(", callbacks:", 12, context);                                                   \
        snprintf(buf, sizeof(buf), "%p", deque.callbacks);                                        \
        string_cb(buf, strlen(buf), context);                                                     \
        string_cb(" }", 2, context);                                                              \
    }                                                                                             \
    void Name##_print(Name##_t deque, FILE* stream, const char* sep) {                            \
        if (deque.callbacks->print_cb) {                                                          \
            for (size_t i = deque.front, j = 0; j < deque.count; ++j) {                           \
                deque.callbacks->print_cb(deque.data[i], stream);                                 \
                if (j != deque.count - 1) {                                                       \
                    fprintf(stream, "%s", sep);                                                   \
                }                                                                                 \
                i = (i + 1) % deque.capacity;                                                     \
            }                                                                                     \
        }                                                                                         \
    }

#endif  // PHYTO_DEQUE_DEQUE_H_

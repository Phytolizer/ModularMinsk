#pragma once

#include <stdint.h>
#include <stdio.h>

#define OBJECT_KINDS_X                                                                             \
    X(NULL)                                                                                        \
    X(INT64)

typedef enum {
#define X(x) OBJECT_KIND_##x,
    OBJECT_KINDS_X
#undef X
} object_kind_t;

typedef struct {
    object_kind_t kind;
    union {
        int64_t int64;
    } as;
} object_t;

#define OBJECT_MAKE_NULL()                                                                         \
    { .kind = OBJECT_KIND_NULL }
#define OBJECT_MAKE_INT64(value)                                                                   \
    {                                                                                              \
        .kind = OBJECT_KIND_INT64, .as = {.int64 = value }                                         \
    }

void object_print(object_t obj, FILE* stream);

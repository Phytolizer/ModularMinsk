#pragma once

#include "minsk/analysis/syntax/kind.h"
#include "minsk/runtime/object.h"
#include <stdint.h>

typedef struct {
    syntax_kind_t kind;
    char* text;
    int64_t position;
    object_t value;
} syntax_token_t;

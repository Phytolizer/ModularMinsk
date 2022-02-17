#include "minsk/analysis/syntax/kind.h"
#include <assert.h>
#include <stdbool.h>

const char* syntax_kind_to_string(syntax_kind_t kind) {
    switch (kind) {
#define X(x)                                                                                       \
    case SYNTAX_KIND_##x:                                                                          \
        return #x;
        SYNTAX_KINDS_X
#undef X
        case SYNTAX_KINDS_COUNT:
        default:
            assert(false && "corrupt syntax kind");
    }
}

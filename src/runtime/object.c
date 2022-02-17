#include "minsk/runtime/object.h"
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>

void object_print(object_t obj, FILE* stream) {
    switch (obj.kind) {
        case OBJECT_KIND_NULL:
            break;
        case OBJECT_KIND_INT64:
            fprintf(stream, "%" PRId64, obj.as.int64);
            break;
        default:
            assert(false && "corrupt object kind");
    }
}

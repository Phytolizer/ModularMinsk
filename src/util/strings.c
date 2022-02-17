#include "minsk/util/strings.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char* string_copy(const char* str) {
    size_t len = strlen(str);
    char* result = calloc(len + 1, 1);
    memcpy(result, str, len);
    return result;
}

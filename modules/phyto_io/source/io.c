#include "phyto/io/io.h"

#include <phyto/string/string.h>
#include <stdio.h>

phyto_string_t phyto_io_read_file(const char* path) {
    phyto_string_t result = phyto_string_new();
    FILE* file = fopen(path, "r");
    if (file) {
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), file)) {
            phyto_string_append_c(&result, buffer);
        }
        fclose(file);
    }
    return result;
}

phyto_string_t phyto_io_read_line(FILE* fp) {
    phyto_string_t result = phyto_string_new();
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp)) {
        phyto_string_append_c(&result, buffer);
        if (result.data[result.size - 1] == '\n') {
            break;
        }
    }
    return result;
}

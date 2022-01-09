#ifndef PHYTO_IO_IO_H_
#define PHYTO_IO_IO_H_

#include <phyto/string/string.h>
#include <stdint.h>
#include <stdio.h>

phyto_string_t phyto_io_read_file(const char* path);
phyto_string_t phyto_io_read_line(FILE* fp);

#endif  // PHYTO_IO_IO_H_

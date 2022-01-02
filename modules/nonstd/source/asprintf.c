#include "nonstd/asprintf.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void NonstdAllocatedStringPrintf(char** result, const char* format, ...) {
  va_list args;
  va_start(args, format);
  int size = vsnprintf(NULL, 0, format, args);
  va_end(args);
  *result = malloc(size + 1);
  va_start(args, format);
  vsnprintf(*result, size + 1, format, args);
  va_end(args);
}

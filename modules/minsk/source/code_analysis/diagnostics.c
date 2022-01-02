#include "minsk/code_analysis/diagnostics.h"

#include <stdint.h>

void MskDiagnosticsFree(MskDiagnostics* diagnostics) {
  for (uint64_t i = 0; i < diagnostics->size; ++i) {
    StringFree(&diagnostics->data[i]);
  }
  VEC_FREE(diagnostics);
}

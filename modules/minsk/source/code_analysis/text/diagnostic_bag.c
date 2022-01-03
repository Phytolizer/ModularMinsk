#include "minsk/code_analysis/text/diagnostic_bag.h"

#include <stdint.h>

void MskDiagnosticBagFree(MskDiagnosticBag* diagnostics) {
  for (uint64_t i = 0; i < diagnostics->size; ++i) {
    MskDiagnosticFree(&diagnostics->data[i]);
  }
  VEC_FREE(diagnostics);
}

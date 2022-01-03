#ifndef MINSK_CODE_ANALYSIS_DIAGNOSTIC_H_
#define MINSK_CODE_ANALYSIS_DIAGNOSTIC_H_

#include <vec/vec.h>

#include "minsk/code_analysis/text/diagnostic.h"

typedef VEC_TYPE(MskDiagnostic) MskDiagnosticBag;

void MskDiagnosticBagFree(MskDiagnosticBag* diagnostics);

#endif  // MINSK_CODE_ANALYSIS_DIAGNOSTIC_H_

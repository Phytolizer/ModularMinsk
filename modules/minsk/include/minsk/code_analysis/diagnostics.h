#ifndef MINSK_CODE_ANALYSIS_DIAGNOSTIC_H_
#define MINSK_CODE_ANALYSIS_DIAGNOSTIC_H_

#include <string/string.h>

typedef VEC_TYPE(String) MskDiagnostics;

void MskDiagnosticsFree(MskDiagnostics* diagnostics);

#endif  // MINSK_CODE_ANALYSIS_DIAGNOSTIC_H_
#ifndef MINSK_CODE_ANALYSIS_TEXT_DIAGNOSTIC_H_
#define MINSK_CODE_ANALYSIS_TEXT_DIAGNOSTIC_H_

#include <string/string.h>

#include "minsk/code_analysis/text/span.h"

typedef struct {
  MskTextSpan span;
  String message;
} MskDiagnostic;

void MskDiagnosticFree(MskDiagnostic* diagnostic);

#endif  // MINSK_CODE_ANALYSIS_TEXT_DIAGNOSTIC_H_

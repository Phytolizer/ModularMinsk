#include "minsk/code_analysis/text/diagnostic.h"

void MskDiagnosticFree(MskDiagnostic* diagnostic) {
  StringFree(&diagnostic->message);
}

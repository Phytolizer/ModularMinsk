#include "minsk/code_analysis/text/diagnostic.h"

#include <phyto/string/string.h>

void MskDiagnosticFree(MskDiagnostic* diagnostic) {
  phyto_string_free(&diagnostic->message);
}

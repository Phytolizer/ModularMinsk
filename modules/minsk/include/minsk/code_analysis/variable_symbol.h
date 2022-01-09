#ifndef MINSK_CODE_ANALYSIS_VARIABLE_SYMBOL_H_
#define MINSK_CODE_ANALYSIS_VARIABLE_SYMBOL_H_

#include <phyto/string/string.h>

#include "minsk/runtime/object.h"

typedef struct {
  phyto_string_t name;
  MskRuntimeObjectKind type;
} MskVariableSymbol;

MskVariableSymbol MskVariableSymbolNew(phyto_string_t name,
                                       MskRuntimeObjectKind type);

#endif  // MINSK_CODE_ANALYSIS_VARIABLE_SYMBOL_H_

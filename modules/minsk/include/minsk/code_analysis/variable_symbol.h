#ifndef MINSK_CODE_ANALYSIS_VARIABLE_SYMBOL_H_
#define MINSK_CODE_ANALYSIS_VARIABLE_SYMBOL_H_

#include <string/string.h>

#include "minsk/runtime/object.h"

typedef struct {
  String name;
  MskRuntimeObjectKind type;
} MskVariableSymbol;

MskVariableSymbol MskVariableSymbolNew(String name, MskRuntimeObjectKind type);

#endif  // MINSK_CODE_ANALYSIS_VARIABLE_SYMBOL_H_

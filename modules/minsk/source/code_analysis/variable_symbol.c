#include "minsk/code_analysis/variable_symbol.h"

MskVariableSymbol MskVariableSymbolNew(phyto_string_t name,
                                       MskRuntimeObjectKind type) {
  return (MskVariableSymbol){
      .name = name,
      .type = type,
  };
}

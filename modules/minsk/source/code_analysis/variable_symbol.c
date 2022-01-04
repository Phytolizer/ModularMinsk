#include "minsk/code_analysis/variable_symbol.h"

MskVariableSymbol MskVariableSymbolNew(String name, MskRuntimeObjectKind type) {
  return (MskVariableSymbol){
      .name = name,
      .type = type,
  };
}

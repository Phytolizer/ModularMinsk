#include "minsk/code_analysis/symbol_table.h"

#include <stdint.h>

#include "hash/hash.h"
#include "minsk/code_analysis/variable_symbol.h"
#include "string/string.h"

static HashKeySpan CreateHashKeySpan(StringView name);

bool MskSymbolTableLookup(MskSymbolTable* table,
                          StringView name,
                          MskSymbolTableEntry* out_value) {
  return HASH_GET(table, CreateHashKeySpan(name), out_value);
}

void MskSymbolTableInsert(MskSymbolTable* table,
                          MskVariableSymbol variable,
                          MskRuntimeObject value) {
  MskSymbolTableEntry entry = {
      .variable = variable,
      .value = value,
  };
  HASH_ADD(table, CreateHashKeySpan(StringAsView(variable.name)), entry);
}

void MskSymbolTableFree(MskSymbolTable* table) {
  HASH_FREE(table);
}

HashKeySpan CreateHashKeySpan(StringView name) {
  return (HashKeySpan){
      .begin = (const uint8_t*)name.begin,
      .end = (const uint8_t*)name.end,
  };
}

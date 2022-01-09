#include "minsk/code_analysis/symbol_table.h"

#include <phyto/hash/hash.h>
#include <phyto/string/string.h>
#include <stdint.h>

#include "minsk/code_analysis/variable_symbol.h"

PHYTO_HASH_IMPL(MskSymbolTable, MskSymbolTableEntry);

bool MskSymbolTableLookup(MskSymbolTable_t* table,
                          phyto_string_span_t name,
                          MskSymbolTableEntry* out_value) {
  *out_value = MskSymbolTable_get(table, name);
  if (MskSymbolTable_flag(table) == phyto_hash_flag_not_found) {
    // handled error, reset flag
    table->flag = phyto_hash_flag_ok;
    return false;
  }
  return true;
}

void MskSymbolTableInsert(MskSymbolTable_t* table,
                          MskVariableSymbol variable,
                          MskRuntimeObject value) {
  MskSymbolTableEntry entry = {
      .variable = variable,
      .value = value,
  };
  MskSymbolTable_insert(table, phyto_string_as_span(variable.name), entry);
}

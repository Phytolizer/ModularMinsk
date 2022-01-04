#include "minsk/code_analysis/symbol_table.h"

#include <stdint.h>

#include "hash/hash.h"

static HashKeySpan CreateHashKeySpan(StringView view);

bool MskSymbolTableLookup(MskSymbolTable* table,
                          StringView name,
                          MskRuntimeObject* out_value) {
  return HASH_GET(table, CreateHashKeySpan(name), out_value);
}

void MskSymbolTableInsert(MskSymbolTable* table,
                          StringView name,
                          MskRuntimeObject value) {
  HASH_ADD(table, CreateHashKeySpan(name), value);
}

void MskSymbolTableFree(MskSymbolTable* table) {
  HASH_FREE(table);
}

HashKeySpan CreateHashKeySpan(StringView view) {
  return (HashKeySpan){
      .begin = (const uint8_t*)view.begin,
      .end = (const uint8_t*)view.end,
  };
}

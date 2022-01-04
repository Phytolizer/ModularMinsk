#ifndef MINSK_CODE_ANALYSIS_SYMBOL_TABLE_H_
#define MINSK_CODE_ANALYSIS_SYMBOL_TABLE_H_

#include <hash/hash.h>
#include <stdbool.h>

#include "minsk/runtime/object.h"

typedef HASH_TYPE(MskRuntimeObject) MskSymbolTable;

bool MskSymbolTableLookup(MskSymbolTable* table,
                          StringView name,
                          MskRuntimeObject* out_value);
void MskSymbolTableInsert(MskSymbolTable* table,
                          StringView name,
                          MskRuntimeObject value);
void MskSymbolTableFree(MskSymbolTable* table);

#endif  // MINSK_CODE_ANALYSIS_SYMBOL_TABLE_H_

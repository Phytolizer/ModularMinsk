#ifndef MINSK_CODE_ANALYSIS_SYMBOL_TABLE_H_
#define MINSK_CODE_ANALYSIS_SYMBOL_TABLE_H_

#include <hash/hash.h>
#include <stdbool.h>

#include "minsk/code_analysis/variable_symbol.h"
#include "minsk/runtime/object.h"

typedef struct {
  MskVariableSymbol variable;
  MskRuntimeObject value;
} MskSymbolTableEntry;

typedef HASH_TYPE(MskSymbolTableEntry) MskSymbolTable;

bool MskSymbolTableLookup(MskSymbolTable* table,
                          StringView name,
                          MskSymbolTableEntry* out_value);
void MskSymbolTableInsert(MskSymbolTable* table,
                          MskVariableSymbol variable,
                          MskRuntimeObject value);
void MskSymbolTableFree(MskSymbolTable* table);

#endif  // MINSK_CODE_ANALYSIS_SYMBOL_TABLE_H_

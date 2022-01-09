#ifndef MINSK_CODE_ANALYSIS_SYMBOL_TABLE_H_
#define MINSK_CODE_ANALYSIS_SYMBOL_TABLE_H_

#include <phyto/hash/hash.h>
#include <phyto/string/string.h>
#include <stdbool.h>

#include "minsk/code_analysis/variable_symbol.h"
#include "minsk/runtime/object.h"

typedef struct {
  MskVariableSymbol variable;
  MskRuntimeObject value;
} MskSymbolTableEntry;

PHYTO_HASH_DECL(MskSymbolTable, MskSymbolTableEntry);

bool MskSymbolTableLookup(MskSymbolTable_t* table,
                          phyto_string_span_t name,
                          MskSymbolTableEntry* out_value);
void MskSymbolTableInsert(MskSymbolTable_t* table,
                          MskVariableSymbol variable,
                          MskRuntimeObject value);

#endif  // MINSK_CODE_ANALYSIS_SYMBOL_TABLE_H_

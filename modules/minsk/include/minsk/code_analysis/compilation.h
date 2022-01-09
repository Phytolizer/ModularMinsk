#ifndef MINSK_CODE_ANALYSIS_COMPILATION_H_
#define MINSK_CODE_ANALYSIS_COMPILATION_H_

#include "minsk/code_analysis/evaluation_result.h"
#include "minsk/code_analysis/symbol_table.h"
#include "minsk/code_analysis/syntax/tree.h"

typedef struct {
  MskSyntaxTree syntax;
} MskCompilation;

MskCompilation MskCompilationNew(MskSyntaxTree syntax);
MskEvaluationResult MskCompilationEvaluate(MskCompilation* compilation,
                                           MskSymbolTable_t* symbols);

#endif  // MINSK_CODE_ANALYSIS_COMPILATION_H_

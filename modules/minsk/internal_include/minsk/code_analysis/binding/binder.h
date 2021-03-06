#ifndef MINSK_CODE_ANALYSIS_BINDING_BINDER_H_
#define MINSK_CODE_ANALYSIS_BINDING_BINDER_H_

#include "minsk/code_analysis/binding/expression.h"
#include "minsk/code_analysis/symbol_table.h"
#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/text/diagnostic_bag.h"

typedef struct {
  MskDiagnosticBag_t diagnostics;
  MskSymbolTable_t* symbols;
} MskBinder;

MskBoundExpression* MskBinderBindExpression(MskBinder* binder,
                                            MskExpressionSyntax* syntax);
void MskBinderFree(MskBinder* binder);

#endif  // MINSK_CODE_ANALYSIS_BINDING_BINDER_H_

#ifndef MINSK_CODE_ANALYSIS_BINDING_BINDER_H_
#define MINSK_CODE_ANALYSIS_BINDING_BINDER_H_

#include "minsk/code_analysis/diagnostics.h"
#include "minsk/code_analysis/syntax/expression.h"
#include "minsk_private/code_analysis/binding/expression.h"

typedef struct {
  MskDiagnostics diagnostics;
} MskBinder;

MskBoundExpression* MskBinderBindExpression(MskBinder* binder,
                                            MskExpressionSyntax* syntax);
void MskBinderFree(MskBinder* binder);

#endif  // MINSK_CODE_ANALYSIS_BINDING_BINDER_H_

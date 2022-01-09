#ifndef MINSK_CODE_ANALYSIS_BINDING_VARIABLE_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_BINDING_VARIABLE_EXPRESSION_H_

#include "minsk/code_analysis/binding/expression.h"
#include "minsk/code_analysis/variable_symbol.h"
#include "minsk/runtime/object.h"

typedef struct {
  MskBoundExpression base;
  MskVariableSymbol variable;
} MskBoundVariableExpression;

MskBoundVariableExpression* MskBoundVariableExpressionNew(
    MskVariableSymbol variable);

#endif  // MINSK_CODE_ANALYSIS_BINDING_VARIABLE_EXPRESSION_H_

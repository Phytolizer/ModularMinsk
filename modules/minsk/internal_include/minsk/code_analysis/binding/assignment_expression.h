#ifndef MINSK_CODE_ANALYSIS_BINDING_ASSIGNMENT_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_BINDING_ASSIGNMENT_EXPRESSION_H_

#include "minsk/code_analysis/binding/expression.h"
#include "minsk/code_analysis/variable_symbol.h"
#include "string/string.h"

typedef struct {
  MskBoundExpression base;
  MskVariableSymbol variable;
  MskBoundExpression* value;
} MskBoundAssignmentExpression;

MskBoundAssignmentExpression* MskBoundAssignmentExpressionNew(
    MskVariableSymbol variable,
    MskBoundExpression* value);

#endif  // MINSK_CODE_ANALYSIS_BINDING_ASSIGNMENT_EXPRESSION_H_

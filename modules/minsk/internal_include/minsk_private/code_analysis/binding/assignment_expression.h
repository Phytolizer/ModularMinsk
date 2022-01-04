#ifndef MINSK_CODE_ANALYSIS_BINDING_ASSIGNMENT_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_BINDING_ASSIGNMENT_EXPRESSION_H_

#include "minsk_private/code_analysis/binding/expression.h"
#include "string/string.h"

typedef struct {
  MskBoundExpression base;
  String name;
  MskBoundExpression* value;
} MskBoundAssignmentExpression;

MskBoundAssignmentExpression* MskBoundAssignmentExpressionNew(
    String name,
    MskBoundExpression* value);

#endif  // MINSK_CODE_ANALYSIS_BINDING_ASSIGNMENT_EXPRESSION_H_

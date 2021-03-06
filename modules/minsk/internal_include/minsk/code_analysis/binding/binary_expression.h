#ifndef MINSK_CODE_ANALYSIS_BINDING_BINARY_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_BINDING_BINARY_EXPRESSION_H_

#include "minsk/code_analysis/binding/binary_operator.h"
#include "minsk/code_analysis/binding/expression.h"

typedef struct {
  MskBoundExpression base;
  MskBoundExpression* left;
  MskBoundBinaryOperator op;
  MskBoundExpression* right;
} MskBoundBinaryExpression;

MskBoundBinaryExpression* MskBoundBinaryExpressionNew(
    MskBoundExpression* left,
    MskBoundBinaryOperator op,
    MskBoundExpression* right);

#endif  // MINSK_CODE_ANALYSIS_BINDING_BINARY_EXPRESSION_H_

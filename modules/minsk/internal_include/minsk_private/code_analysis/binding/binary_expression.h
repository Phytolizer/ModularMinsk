#ifndef MINSK_CODE_ANALYSIS_BINDING_BINARY_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_BINDING_BINARY_EXPRESSION_H_

#include "minsk_private/code_analysis/binding/binary_operator_kind.h"
#include "minsk_private/code_analysis/binding/expression.h"

typedef struct {
  MskBoundExpression base;
  MskBoundExpression* left;
  MskBoundBinaryOperatorKind operator_kind;
  MskBoundExpression* right;
} MskBoundBinaryExpression;

MskBoundBinaryExpression* MskBoundBinaryExpressionNew(
    MskBoundExpression* left,
    MskBoundBinaryOperatorKind operator_kind,
    MskBoundExpression* right);

#endif  // MINSK_CODE_ANALYSIS_BINDING_BINARY_EXPRESSION_H_

#ifndef MINSK_CODE_ANALYSIS_BINDING_UNARY_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_BINDING_UNARY_EXPRESSION_H_

#include "minsk_private/code_analysis/binding/expression.h"
#include "minsk_private/code_analysis/binding/unary_operator_kind.h"

typedef struct {
  MskBoundExpression base;
  MskBoundUnaryOperatorKind operator_kind;
  MskBoundExpression* operand;
} MskBoundUnaryExpression;

MskBoundUnaryExpression* MskBoundUnaryExpressionNew(
    MskBoundUnaryOperatorKind operator_kind,
    MskBoundExpression* operand);

#endif  // MINSK_CODE_ANALYSIS_BINDING_UNARY_EXPRESSION_H_

#ifndef MINSK_CODE_ANALYSIS_BINDING_UNARY_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_BINDING_UNARY_EXPRESSION_H_

#include "minsk/code_analysis/binding/expression.h"
#include "minsk/code_analysis/binding/unary_operator.h"
#include "minsk/code_analysis/binding/unary_operator_kind.h"

typedef struct {
  MskBoundExpression base;
  MskBoundUnaryOperator op;
  MskBoundExpression* operand;
} MskBoundUnaryExpression;

MskBoundUnaryExpression* MskBoundUnaryExpressionNew(
    MskBoundUnaryOperator op,
    MskBoundExpression* operand);

#endif  // MINSK_CODE_ANALYSIS_BINDING_UNARY_EXPRESSION_H_

#include "minsk_private/code_analysis/binding/unary_expression.h"

#include "minsk_private/code_analysis/binding/node.h"

MskBoundUnaryExpression* MskBoundUnaryExpressionNew(
    MskBoundUnaryOperatorKind operator_kind,
    MskBoundExpression* operand) {
  MskBoundUnaryExpression* exp = calloc(1, sizeof(MskBoundUnaryExpression));
  exp->base.base.cls = kMskBoundNodeClassExpression;
  exp->base.cls = kMskBoundExpressionClassUnary;
  exp->operator_kind = operator_kind;
  exp->operand = operand;
  return exp;
}

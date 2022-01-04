#include "minsk/code_analysis/binding/unary_expression.h"

#include "minsk/code_analysis/binding/node.h"

MskBoundUnaryExpression* MskBoundUnaryExpressionNew(
    MskBoundUnaryOperator op,
    MskBoundExpression* operand) {
  MskBoundUnaryExpression* exp = calloc(1, sizeof(MskBoundUnaryExpression));
  exp->base.base.cls = kMskBoundNodeClassExpression;
  exp->base.cls = kMskBoundExpressionClassUnary;
  exp->op = op;
  exp->operand = operand;
  return exp;
}

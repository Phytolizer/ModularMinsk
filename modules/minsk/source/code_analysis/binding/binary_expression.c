#include "minsk_private/code_analysis/binding/binary_expression.h"

MskBoundBinaryExpression* MskBoundBinaryExpressionNew(
    MskBoundExpression* left,
    MskBoundBinaryOperatorKind operator_kind,
    MskBoundExpression* right) {
  MskBoundBinaryExpression* exp = calloc(1, sizeof(MskBoundBinaryExpression));
  exp->base.base.cls = kMskBoundNodeClassExpression;
  exp->base.cls = kMskBoundExpressionClassBinary;
  exp->left = left;
  exp->operator_kind = operator_kind;
  exp->right = right;
  return exp;
}

#include "minsk_private/code_analysis/binding/literal_expression.h"

MskBoundLiteralExpression* MskBoundLiteralExpressionNew(
    MskRuntimeObject* value) {
  MskBoundLiteralExpression* exp = calloc(1, sizeof(MskBoundLiteralExpression));
  exp->base.base.cls = kMskBoundNodeClassExpression;
  exp->base.cls = kMskBoundExpressionClassLiteral;
  exp->value = value;
  return exp;
}

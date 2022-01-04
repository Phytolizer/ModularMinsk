#include "minsk_private/code_analysis/binding/assignment_expression.h"

MskBoundAssignmentExpression* MskBoundAssignmentExpressionNew(
    String name,
    MskBoundExpression* value) {
  MskBoundAssignmentExpression* exp =
      calloc(sizeof(MskBoundAssignmentExpression), 1);
  exp->base.base.cls = kMskBoundNodeClassExpression;
  exp->base.cls = kMskBoundExpressionClassAssignment;
  exp->name = name;
  exp->value = value;
  return exp;
}

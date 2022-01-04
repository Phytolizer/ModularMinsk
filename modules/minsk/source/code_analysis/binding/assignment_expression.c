#include "minsk/code_analysis/binding/assignment_expression.h"

MskBoundAssignmentExpression* MskBoundAssignmentExpressionNew(
    MskVariableSymbol variable,
    MskBoundExpression* value) {
  MskBoundAssignmentExpression* exp =
      calloc(sizeof(MskBoundAssignmentExpression), 1);
  exp->base.base.cls = kMskBoundNodeClassExpression;
  exp->base.cls = kMskBoundExpressionClassAssignment;
  exp->variable = variable;
  exp->value = value;
  return exp;
}

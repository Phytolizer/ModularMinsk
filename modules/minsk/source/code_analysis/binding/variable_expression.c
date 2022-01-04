#include "minsk/code_analysis/binding/variable_expression.h"

#include <stdlib.h>

#include "minsk/code_analysis/binding/expression.h"
#include "minsk/code_analysis/binding/node.h"

MskBoundVariableExpression* MskBoundVariableExpressionNew(
    MskVariableSymbol variable) {
  MskBoundVariableExpression* exp =
      calloc(sizeof(MskBoundVariableExpression), 1);
  exp->base.base.cls = kMskBoundNodeClassExpression;
  exp->base.cls = kMskBoundExpressionClassVariable;
  exp->variable = variable;
  return exp;
}

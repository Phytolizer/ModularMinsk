#include "minsk/code_analysis/binding/variable_expression.h"

#include <stdlib.h>

#include "minsk/code_analysis/binding/expression.h"
#include "minsk/code_analysis/binding/node.h"

MskBoundVariableExpression* MskBoundVariableExpressionNew(
    String name,
    MskRuntimeObjectKind type) {
  MskBoundVariableExpression* exp =
      calloc(sizeof(MskBoundVariableExpression), 1);
  exp->base.base.cls = kMskBoundNodeClassExpression;
  exp->base.cls = kMskBoundExpressionClassVariable;
  exp->name = name;
  exp->type = type;
  return exp;
}

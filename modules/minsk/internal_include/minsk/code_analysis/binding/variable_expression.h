#ifndef MINSK_CODE_ANALYSIS_BINDING_VARIABLE_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_BINDING_VARIABLE_EXPRESSION_H_

#include "minsk/runtime/object.h"
#include "minsk/code_analysis/binding/expression.h"
#include "string/string.h"

typedef struct {
  MskBoundExpression base;
  String name;
  MskRuntimeObjectKind type;
} MskBoundVariableExpression;

MskBoundVariableExpression* MskBoundVariableExpressionNew(
    String name,
    MskRuntimeObjectKind type);

#endif  // MINSK_CODE_ANALYSIS_BINDING_VARIABLE_EXPRESSION_H_

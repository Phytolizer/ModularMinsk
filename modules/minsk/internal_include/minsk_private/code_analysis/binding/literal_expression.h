#ifndef MINSK_CODE_ANALYSIS_BINDING_LITERAL_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_BINDING_LITERAL_EXPRESSION_H_

#include "minsk/runtime/object.h"
#include "minsk_private/code_analysis/binding/expression.h"

typedef struct {
  MskBoundExpression base;
  MskRuntimeObject* value;
} MskBoundLiteralExpression;

MskBoundLiteralExpression* MskBoundLiteralExpressionNew(
    MskRuntimeObject* value);

#endif  // MINSK_CODE_ANALYSIS_BINDING_LITERAL_EXPRESSION_H_

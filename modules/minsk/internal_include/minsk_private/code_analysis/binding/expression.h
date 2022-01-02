#ifndef MINSK_CODE_ANALYSIS_BINDING_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_BINDING_EXPRESSION_H_

#include "minsk/runtime/object.h"
#include "minsk_private/code_analysis/binding/node.h"

#define MINSK__BOUND_EXPRESSION_CLASSES \
  X(Literal)                            \
  X(Binary)                             \
  X(Unary)

typedef enum {
  kMskBoundExpressionClassInvalid,
#define X(x) kMskBoundExpressionClass##x,
  MINSK__BOUND_EXPRESSION_CLASSES
#undef X
} MskBoundExpressionClass;

typedef struct {
  MskBoundNode base;
  MskBoundExpressionClass cls;
} MskBoundExpression;

MskRuntimeObjectKind MskBoundExpressionGetType(MskBoundExpression* exp);

#endif  // MINSK_CODE_ANALYSIS_BINDING_EXPRESSION_H_

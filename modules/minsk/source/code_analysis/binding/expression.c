#include "minsk_private/code_analysis/binding/expression.h"

#include "minsk/runtime/object.h"
#include "minsk_private/code_analysis/binding/binary_expression.h"
#include "minsk_private/code_analysis/binding/literal_expression.h"
#include "minsk_private/code_analysis/binding/unary_expression.h"

static MskRuntimeObjectKind UnaryExpressionGetType(
    MskBoundUnaryExpression* exp);
static MskRuntimeObjectKind LiteralExpressionGetType(
    MskBoundLiteralExpression* exp);
static MskRuntimeObjectKind BinaryExpressionGetType(
    MskBoundBinaryExpression* exp);

MskRuntimeObjectKind MskBoundExpressionGetType(MskBoundExpression* exp) {
  switch (exp->cls) {
#define X(x)                        \
  case kMskBoundExpressionClass##x: \
    return x##ExpressionGetType((MskBound##x##Expression*)exp);
    MINSK__BOUND_EXPRESSION_CLASSES
#undef X
    case kMskBoundExpressionClassInvalid:
    default:
      return kMskObjectKindInvalid;
  }
}

MskRuntimeObjectKind UnaryExpressionGetType(MskBoundUnaryExpression* exp) {
  return MskBoundExpressionGetType(exp->operand);
}

MskRuntimeObjectKind LiteralExpressionGetType(MskBoundLiteralExpression* exp) {
  return exp->value.kind;
}

MskRuntimeObjectKind BinaryExpressionGetType(MskBoundBinaryExpression* exp) {
  return MskBoundExpressionGetType(exp->left);
}

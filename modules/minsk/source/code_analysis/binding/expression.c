#include "minsk/code_analysis/binding/expression.h"

#include "minsk/code_analysis/binding/assignment_expression.h"
#include "minsk/code_analysis/binding/binary_expression.h"
#include "minsk/code_analysis/binding/literal_expression.h"
#include "minsk/code_analysis/binding/unary_expression.h"
#include "minsk/code_analysis/binding/variable_expression.h"
#include "minsk/runtime/object.h"

static MskRuntimeObjectKind UnaryExpressionGetType(
    MskBoundUnaryExpression* exp);
static MskRuntimeObjectKind LiteralExpressionGetType(
    MskBoundLiteralExpression* exp);
static MskRuntimeObjectKind BinaryExpressionGetType(
    MskBoundBinaryExpression* exp);
static MskRuntimeObjectKind VariableExpressionGetType(
    MskBoundVariableExpression* exp);
static MskRuntimeObjectKind AssignmentExpressionGetType(
    MskBoundAssignmentExpression* exp);

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
  return exp->op.result_type;
}

MskRuntimeObjectKind LiteralExpressionGetType(MskBoundLiteralExpression* exp) {
  return exp->value.kind;
}

MskRuntimeObjectKind BinaryExpressionGetType(MskBoundBinaryExpression* exp) {
  return exp->op.result_type;
}

MskRuntimeObjectKind VariableExpressionGetType(
    MskBoundVariableExpression* exp) {
  return exp->variable.type;
}

MskRuntimeObjectKind AssignmentExpressionGetType(
    MskBoundAssignmentExpression* exp) {
  return MskBoundExpressionGetType(exp->value);
}

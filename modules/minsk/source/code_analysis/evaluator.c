#include "minsk_private/code_analysis/evaluator.h"

#include <assert.h>
#include <stdio.h>

#include "minsk/code_analysis/symbol_table.h"
#include "minsk/runtime/object.h"
#include "minsk_private/code_analysis/binding/assignment_expression.h"
#include "minsk_private/code_analysis/binding/binary_expression.h"
#include "minsk_private/code_analysis/binding/binary_operator_kind.h"
#include "minsk_private/code_analysis/binding/binder.h"
#include "minsk_private/code_analysis/binding/expression.h"
#include "minsk_private/code_analysis/binding/literal_expression.h"
#include "minsk_private/code_analysis/binding/node.h"
#include "minsk_private/code_analysis/binding/unary_expression.h"
#include "minsk_private/code_analysis/binding/unary_operator_kind.h"
#include "minsk_private/code_analysis/binding/variable_expression.h"

static MskRuntimeObject EvaluateExpression(MskEvaluator* evaluator,
                                           MskBoundExpression* expression);
static MskRuntimeObject EvaluateLiteralExpression(
    MskEvaluator* evaluator,
    MskBoundLiteralExpression* expression);
static MskRuntimeObject EvaluateBinaryExpression(
    MskEvaluator* evaluator,
    MskBoundBinaryExpression* expression);
static MskRuntimeObject EvaluateUnaryExpression(
    MskEvaluator* evaluator,
    MskBoundUnaryExpression* expression);
static MskRuntimeObject EvaluateVariableExpression(
    MskEvaluator* evaluator,
    MskBoundVariableExpression* expression);
static MskRuntimeObject EvaluateAssignmentExpression(
    MskEvaluator* evaluator,
    MskBoundAssignmentExpression* expression);

MskEvaluator MskEvaluatorNew(MskBoundExpression* root,
                             MskSymbolTable* symbols) {
  return (MskEvaluator){
      .root = root,
      .symbols = symbols,
  };
}

MskRuntimeObject MskEvaluatorEvaluate(MskEvaluator* evaluator) {
  return EvaluateExpression(evaluator, evaluator->root);
}

MskRuntimeObject EvaluateExpression(MskEvaluator* evaluator,
                                    MskBoundExpression* expression) {
  switch (expression->cls) {
#define X(x)                                  \
  case kMskBoundExpressionClass##x:           \
    return Evaluate##x##Expression(evaluator, \
                                   (MskBound##x##Expression*)expression);
    MINSK__BOUND_EXPRESSION_CLASSES
#undef X
    default:
      assert(false && "corrupt bound expression");
      return (MskRuntimeObject){0};
  }
}

MskRuntimeObject EvaluateLiteralExpression(
    MskEvaluator* evaluator,
    MskBoundLiteralExpression* expression) {
  (void)evaluator;
  return expression->value;
}

MskRuntimeObject EvaluateBinaryExpression(
    MskEvaluator* evaluator,
    MskBoundBinaryExpression* expression) {
  MskRuntimeObject left = EvaluateExpression(evaluator, expression->left);
  MskRuntimeObject right = EvaluateExpression(evaluator, expression->right);
  switch (expression->op.kind) {
    case kMskBoundBinaryOperatorKindAddition:
      return MskRuntimeObjectNewInteger(left.value.integer +
                                        right.value.integer);
    case kMskBoundBinaryOperatorKindSubtraction:
      return MskRuntimeObjectNewInteger(left.value.integer -
                                        right.value.integer);
    case kMskBoundBinaryOperatorKindMultiplication:
      return MskRuntimeObjectNewInteger(left.value.integer *
                                        right.value.integer);
    case kMskBoundBinaryOperatorKindDivision:
      return MskRuntimeObjectNewInteger(left.value.integer /
                                        right.value.integer);
    case kMskBoundBinaryOperatorKindLogicalAnd:
      return MskRuntimeObjectNewBoolean(left.value.boolean &&
                                        right.value.boolean);
    case kMskBoundBinaryOperatorKindLogicalOr:
      return MskRuntimeObjectNewBoolean(left.value.boolean ||
                                        right.value.boolean);
    case kMskBoundBinaryOperatorKindEquality:
      return MskRuntimeObjectNewBoolean(MskRuntimeObjectEquals(&left, &right));
    case kMskBoundBinaryOperatorKindInequality:
      return MskRuntimeObjectNewBoolean(!MskRuntimeObjectEquals(&left, &right));
    default:
      assert(false && "corrupt binary operator kind");
      return (MskRuntimeObject){0};
  }
}

MskRuntimeObject EvaluateUnaryExpression(MskEvaluator* evaluator,
                                         MskBoundUnaryExpression* expression) {
  MskRuntimeObject operand = EvaluateExpression(evaluator, expression->operand);
  switch (expression->op.kind) {
    case kMskBoundUnaryOperatorKindIdentity:
      return MskRuntimeObjectNewInteger(operand.value.integer);
    case kMskBoundUnaryOperatorKindNegation:
      return MskRuntimeObjectNewInteger(-operand.value.integer);
    case kMskBoundUnaryOperatorKindLogicalNegation:
      return MskRuntimeObjectNewBoolean(!operand.value.boolean);
    default:
      assert(false && "corrupt unary operator kind");
      return (MskRuntimeObject){0};
  }
}

MskRuntimeObject EvaluateVariableExpression(
    MskEvaluator* evaluator,
    MskBoundVariableExpression* expression) {
  MskRuntimeObject value = {0};
  MskSymbolTableLookup(evaluator->symbols, StringAsView(expression->name),
                       &value);
  return value;
}

MskRuntimeObject EvaluateAssignmentExpression(
    MskEvaluator* evaluator,
    MskBoundAssignmentExpression* expression) {
  MskRuntimeObject value = EvaluateExpression(evaluator, expression->value);
  MskSymbolTableInsert(evaluator->symbols, StringAsView(expression->name),
                       value);
  return value;
}

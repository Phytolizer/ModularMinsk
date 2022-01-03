#include "minsk/code_analysis/evaluator.h"

#include <assert.h>
#include <stdio.h>

#include "minsk/code_analysis/syntax/binary_expression.h"
#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/literal_expression.h"
#include "minsk/code_analysis/syntax/parenthesized_expression.h"
#include "minsk/code_analysis/syntax/unary_expression.h"
#include "minsk/runtime/object.h"
#include "minsk_private/code_analysis/binding/binary_expression.h"
#include "minsk_private/code_analysis/binding/binary_operator_kind.h"
#include "minsk_private/code_analysis/binding/binder.h"
#include "minsk_private/code_analysis/binding/expression.h"
#include "minsk_private/code_analysis/binding/literal_expression.h"
#include "minsk_private/code_analysis/binding/node.h"
#include "minsk_private/code_analysis/binding/unary_expression.h"
#include "minsk_private/code_analysis/binding/unary_operator_kind.h"

struct MskEvaluatorImpl {
  MskBoundExpression* root;
};

static MskEvaluatorImpl* MskEvaluatorImplNew(MskExpressionSyntax* root);
static void MskEvaluatorImplFree(MskEvaluatorImpl* impl);

static MskRuntimeObject EvaluateExpression(MskBoundExpression* expression);
static MskRuntimeObject EvaluateLiteralExpression(
    MskBoundLiteralExpression* expression);
static MskRuntimeObject EvaluateBinaryExpression(
    MskBoundBinaryExpression* expression);
static MskRuntimeObject EvaluateUnaryExpression(
    MskBoundUnaryExpression* expression);

MskEvaluator MskEvaluatorNew(MskExpressionSyntax* root) {
  return (MskEvaluator){
      .impl = MskEvaluatorImplNew(root),
  };
}

MskRuntimeObject MskEvaluatorEvaluate(MskEvaluator* evaluator) {
  return EvaluateExpression(evaluator->impl->root);
}

void MskEvaluatorFree(MskEvaluator* e) {
  MskEvaluatorImplFree(e->impl);
  free(e->impl);
  e->impl = NULL;
}

MskEvaluatorImpl* MskEvaluatorImplNew(MskExpressionSyntax* root) {
  MskBinder binder = {0};
  MskBoundExpression* bound_root = MskBinderBindExpression(&binder, root);
  MskBinderFree(&binder);
  MskEvaluatorImpl* impl = calloc(1, sizeof(MskEvaluatorImpl));
  impl->root = bound_root;
  return impl;
}

void MskEvaluatorImplFree(MskEvaluatorImpl* impl) {
  MskBoundNodeFree(&impl->root->base);
  free(impl->root);
}

MskRuntimeObject EvaluateExpression(MskBoundExpression* expression) {
  switch (expression->cls) {
    case kMskBoundExpressionClassLiteral:
      return EvaluateLiteralExpression((MskBoundLiteralExpression*)expression);
    case kMskBoundExpressionClassBinary:
      return EvaluateBinaryExpression((MskBoundBinaryExpression*)expression);
    case kMskBoundExpressionClassUnary:
      return EvaluateUnaryExpression((MskBoundUnaryExpression*)expression);
    default:
      assert(false && "corrupt bound expression");
      return (MskRuntimeObject){0};
  }
}

MskRuntimeObject EvaluateLiteralExpression(
    MskBoundLiteralExpression* expression) {
  return expression->value;
}

MskRuntimeObject EvaluateBinaryExpression(
    MskBoundBinaryExpression* expression) {
  MskRuntimeObject left = EvaluateExpression(expression->left);
  MskRuntimeObject right = EvaluateExpression(expression->right);
  assert(left.kind == kMskObjectKindInteger &&
         right.kind == kMskObjectKindInteger && "invalid operands");
  switch (expression->operator_kind) {
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
    default:
      assert(false && "corrupt binary operator kind");
      return (MskRuntimeObject){0};
  }
}

MskRuntimeObject EvaluateUnaryExpression(MskBoundUnaryExpression* expression) {
  MskRuntimeObject operand = EvaluateExpression(expression->operand);
  assert(operand.kind == kMskObjectKindInteger && "invalid operand");
  switch (expression->operator_kind) {
    case kMskBoundUnaryOperatorKindIdentity:
      return MskRuntimeObjectNewInteger(operand.value.integer);
    case kMskBoundUnaryOperatorKindNegation:
      return MskRuntimeObjectNewInteger(-operand.value.integer);
    default:
      assert(false && "corrupt unary operator kind");
      return (MskRuntimeObject){0};
  }
}

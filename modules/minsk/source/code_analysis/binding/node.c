#include "minsk/code_analysis/binding/node.h"

#include <stdlib.h>

#include "minsk/runtime/object.h"
#include "minsk/code_analysis/binding/assignment_expression.h"
#include "minsk/code_analysis/binding/binary_expression.h"
#include "minsk/code_analysis/binding/expression.h"
#include "minsk/code_analysis/binding/kind.h"
#include "minsk/code_analysis/binding/literal_expression.h"
#include "minsk/code_analysis/binding/unary_expression.h"
#include "minsk/code_analysis/binding/variable_expression.h"

static MskBoundNodeKind GetExpressionKind(MskBoundNode* node);
static void FreeExpression(MskBoundNode* node);

static void FreeLiteralExpression(MskBoundExpression* exp);
static void FreeUnaryExpression(MskBoundExpression* exp);
static void FreeBinaryExpression(MskBoundExpression* exp);
static void FreeVariableExpression(MskBoundExpression* exp);
static void FreeAssignmentExpression(MskBoundExpression* exp);

MskBoundNodeKind MskBoundNodeGetKind(MskBoundNode* node) {
  switch (node->cls) {
    case kMskBoundNodeClassInvalid:
      return kMskBoundNodeKindInvalid;
#define X(x)                  \
  case kMskBoundNodeClass##x: \
    return Get##x##Kind(node);
      MINSK__BOUND_NODE_CLASSES
#undef X
    default:
      return kMskBoundNodeKindInvalid;
  }
}

void MskBoundNodeFree(MskBoundNode* node) {
  if (node == NULL) {
    return;
  }
  switch (node->cls) {
#define X(x)                  \
  case kMskBoundNodeClass##x: \
    return Free##x(node);
    MINSK__BOUND_NODE_CLASSES
#undef X
    default:
      return;
  }
}

MskBoundNodeKind GetExpressionKind(MskBoundNode* node) {
  MskBoundExpression* exp = (MskBoundExpression*)node;
  switch (exp->cls) {
    case kMskBoundExpressionClassInvalid:
      return kMskBoundNodeKindInvalid;
#define X(x)                        \
  case kMskBoundExpressionClass##x: \
    return kMskBoundNodeKind##x##Expression;
      MINSK__BOUND_EXPRESSION_CLASSES
#undef X
    default:
      return kMskBoundNodeKindInvalid;
  }
}

void FreeExpression(MskBoundNode* node) {
  MskBoundExpression* exp = (MskBoundExpression*)node;
  switch (exp->cls) {
#define X(x)                        \
  case kMskBoundExpressionClass##x: \
    return Free##x##Expression(exp);
    MINSK__BOUND_EXPRESSION_CLASSES
#undef X
    default:
      return;
  }
}

void FreeLiteralExpression(MskBoundExpression* exp) {
  MskBoundLiteralExpression* lit = (MskBoundLiteralExpression*)exp;
  MskRuntimeObjectFree(&lit->value);
}

void FreeUnaryExpression(MskBoundExpression* exp) {
  MskBoundUnaryExpression* unary = (MskBoundUnaryExpression*)exp;
  FreeExpression(&unary->operand->base);
  free(unary->operand);
  unary->operand = NULL;
}

void FreeBinaryExpression(MskBoundExpression* exp) {
  MskBoundBinaryExpression* binary = (MskBoundBinaryExpression*)exp;
  FreeExpression(&binary->left->base);
  FreeExpression(&binary->right->base);
  free(binary->left);
  free(binary->right);
  binary->left = NULL;
  binary->right = NULL;
}

void FreeVariableExpression(MskBoundExpression* exp) {
  MskBoundVariableExpression* variable = (MskBoundVariableExpression*)exp;
  StringFree(&variable->name);
}

void FreeAssignmentExpression(MskBoundExpression* exp) {
  MskBoundAssignmentExpression* assignment = (MskBoundAssignmentExpression*)exp;
  StringFree(&assignment->name);
  FreeExpression(&assignment->value->base);
  free(assignment->value);
  assignment->value = NULL;
}

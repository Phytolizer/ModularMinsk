#include "minsk_private/code_analysis/binding/node.h"

#include <stdlib.h>

#include "minsk/runtime/object.h"
#include "minsk_private/code_analysis/binding/binary_expression.h"
#include "minsk_private/code_analysis/binding/expression.h"
#include "minsk_private/code_analysis/binding/kind.h"
#include "minsk_private/code_analysis/binding/literal_expression.h"
#include "minsk_private/code_analysis/binding/unary_expression.h"

static MskBoundNodeKind BoundExpressionGetKind(MskBoundNode* node);
static void FreeExpression(MskBoundNode* node);

static void FreeLiteralExpression(MskBoundExpression* exp);
static void FreeUnaryExpression(MskBoundExpression* exp);
static void FreeBinaryExpression(MskBoundExpression* exp);

MskBoundNodeKind MskBoundNodeGetKind(MskBoundNode* node) {
  switch (node->cls) {
    case kMskBoundNodeClassInvalid:
      return kMskBoundNodeKindInvalid;
#define X(x)                  \
  case kMskBoundNodeClass##x: \
    return Bound##x##GetKind(node);
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

MskBoundNodeKind BoundExpressionGetKind(MskBoundNode* node) {
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
  MskRuntimeObjectFree(lit->value);
  free(lit->value);
}

void FreeUnaryExpression(MskBoundExpression* exp) {
  MskBoundUnaryExpression* unary = (MskBoundUnaryExpression*)exp;
  FreeExpression(&unary->operand->base);
  free(unary->operand);
}

void FreeBinaryExpression(MskBoundExpression* exp) {
  MskBoundBinaryExpression* binary = (MskBoundBinaryExpression*)exp;
  FreeExpression(&binary->left->base);
  FreeExpression(&binary->right->base);
  free(binary->left);
  free(binary->right);
}

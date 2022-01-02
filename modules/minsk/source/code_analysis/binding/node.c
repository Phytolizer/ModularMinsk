#include "minsk_private/code_analysis/binding/node.h"

#include "minsk_private/code_analysis/binding/expression.h"
#include "minsk_private/code_analysis/binding/kind.h"

static MskBoundNodeKind BoundExpressionGetKind(MskBoundNode* node);

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

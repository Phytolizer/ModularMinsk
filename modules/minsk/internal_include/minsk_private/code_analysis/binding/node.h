#ifndef MINSK_CODE_ANALYSIS_BINDING_NODE_H_
#define MINSK_CODE_ANALYSIS_BINDING_NODE_H_

#include "minsk_private/code_analysis/binding/kind.h"

#define MINSK__BOUND_NODE_CLASSES X(Expression)

/// The actual type of the node, in the C "class" hierarchy.
///
/// \see MskBoundNodeKind for the flattened list of all node kinds.
typedef enum {
  kMskBoundNodeClassInvalid,
#define X(x) kMskBoundNodeClass##x,
  MINSK__BOUND_NODE_CLASSES
#undef X
} MskBoundNodeClass;

typedef struct {
  MskBoundNodeClass cls;
} MskBoundNode;

/// Dispatches on the node class to determine the kind.
MskBoundNodeKind MskBoundNodeGetKind(MskBoundNode* node);
/// Dispatches on the node class to free the node.
void MskBoundNodeFree(MskBoundNode* node);

#endif  // MINSK_CODE_ANALYSIS_BINDING_NODE_H_

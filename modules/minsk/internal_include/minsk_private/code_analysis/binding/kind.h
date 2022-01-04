#ifndef MINSK_CODE_ANALYSIS_BINDING_KIND_H_
#define MINSK_CODE_ANALYSIS_BINDING_KIND_H_

#define MINSK__BOUND_NODE_KINDS \
  X(Invalid)                    \
  X(BinaryExpression)           \
  X(LiteralExpression)          \
  X(UnaryExpression)            \
  X(VariableExpression)         \
  X(AssignmentExpression)

/// The concrete node kind.
///
/// \see MskBoundNodeClass for the hierarchical node kind (i.e. Expression vs.
/// Statement).
typedef enum {
#define X(x) kMskBoundNodeKind##x,
  MINSK__BOUND_NODE_KINDS
#undef X
} MskBoundNodeKind;

#endif  // MINSK_CODE_ANALYSIS_BINDING_KIND_H_

#ifndef MINSK_CODE_ANALYSIS_BINDING_UNARY_OPERATOR_KIND_H_
#define MINSK_CODE_ANALYSIS_BINDING_UNARY_OPERATOR_KIND_H_

#define MINSK__BOUND_UNARY_OPERATOR_KINDS \
  X(Identity)                             \
  X(Negation)                             \
  X(LogicalNegation)

typedef enum {
  kMskBoundUnaryOperatorKindInvalid,
#define X(x) kMskBoundUnaryOperatorKind##x,
  MINSK__BOUND_UNARY_OPERATOR_KINDS
#undef X
} MskBoundUnaryOperatorKind;

#endif  // MINSK_CODE_ANALYSIS_BINDING_UNARY_OPERATOR_KIND_H_

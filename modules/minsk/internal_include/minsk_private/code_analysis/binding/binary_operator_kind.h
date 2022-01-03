#ifndef MINSK_CODE_ANALYSIS_BINDING_BINARY_OPERATOR_KIND_H_
#define MINSK_CODE_ANALYSIS_BINDING_BINARY_OPERATOR_KIND_H_

#define MINSK__BOUND_BINARY_OPERATOR_KINDS \
  X(Addition)                              \
  X(Subtraction)                           \
  X(Multiplication)                        \
  X(Division)                              \
  X(LogicalAnd)                            \
  X(LogicalOr)                             \
  X(Equality)                              \
  X(Inequality)

typedef enum {
  kMskBoundBinaryOperatorKindInvalid,
#define X(x) kMskBoundBinaryOperatorKind##x,
  MINSK__BOUND_BINARY_OPERATOR_KINDS
#undef X
} MskBoundBinaryOperatorKind;

#endif  // MINSK_CODE_ANALYSIS_BINDING_BINARY_OPERATOR_KIND_H_

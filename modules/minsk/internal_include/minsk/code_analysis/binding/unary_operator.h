#ifndef MINSK_CODE_ANALYSIS_BINDING_UNARY_OPERATOR_H_
#define MINSK_CODE_ANALYSIS_BINDING_UNARY_OPERATOR_H_

#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/runtime/object.h"
#include "minsk/code_analysis/binding/unary_operator_kind.h"

typedef struct {
  MskSyntaxKind syntax_kind;
  MskBoundUnaryOperatorKind kind;
  MskRuntimeObjectKind operand_type;
  MskRuntimeObjectKind result_type;
} MskBoundUnaryOperator;

const MskBoundUnaryOperator* MskBindUnaryOperator(MskSyntaxKind syntax_kind,
                                            MskRuntimeObjectKind operand_type);

#endif  // MINSK_CODE_ANALYSIS_BINDING_UNARY_OPERATOR_H_

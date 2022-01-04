#ifndef MINSK_CODE_ANALYSIS_BINDING_BINARY_OPERATOR_H_
#define MINSK_CODE_ANALYSIS_BINDING_BINARY_OPERATOR_H_

#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/runtime/object.h"
#include "minsk/code_analysis/binding/binary_operator_kind.h"

typedef struct {
  MskSyntaxKind syntax_kind;
  MskBoundBinaryOperatorKind kind;
  MskRuntimeObjectKind left_type;
  MskRuntimeObjectKind right_type;
  MskRuntimeObjectKind result_type;
} MskBoundBinaryOperator;

const MskBoundBinaryOperator* MskBindBinaryOperator(
    MskSyntaxKind syntax_kind,
    MskRuntimeObjectKind left_type,
    MskRuntimeObjectKind right_type);

#endif  // MINSK_CODE_ANALYSIS_BINDING_BINARY_OPERATOR_H_

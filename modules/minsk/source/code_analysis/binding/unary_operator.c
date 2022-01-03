#include "minsk_private/code_analysis/binding/unary_operator.h"

#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/runtime/object.h"
#include "minsk_private/code_analysis/binding/unary_operator_kind.h"

static const MskBoundUnaryOperator kOperators[] = {
    {
        .syntax_kind = kMskSyntaxKindPlusToken,
        .kind = kMskBoundUnaryOperatorKindIdentity,
        .operand_type = kMskObjectKindInteger,
        .result_type = kMskObjectKindInteger,
    },
    {
        .syntax_kind = kMskSyntaxKindMinusToken,
        .kind = kMskBoundUnaryOperatorKindNegation,
        .operand_type = kMskObjectKindInteger,
        .result_type = kMskObjectKindInteger,
    },
    {
        .syntax_kind = kMskSyntaxKindBangToken,
        .kind = kMskBoundUnaryOperatorKindLogicalNegation,
        .operand_type = kMskObjectKindBoolean,
        .result_type = kMskObjectKindBoolean,
    },
};

const MskBoundUnaryOperator* MskBindUnaryOperator(
    MskSyntaxKind syntax_kind,
    MskRuntimeObjectKind operand_type) {
  for (size_t i = 0; i < sizeof(kOperators) / sizeof(MskBoundUnaryOperator);
       ++i) {
    const MskBoundUnaryOperator* op = &kOperators[i];
    if (op->syntax_kind == syntax_kind && op->operand_type == operand_type) {
      return op;
    }
  }
  return NULL;
}

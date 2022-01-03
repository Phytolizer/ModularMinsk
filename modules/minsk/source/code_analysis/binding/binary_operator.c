#include "minsk_private/code_analysis/binding/binary_operator.h"

#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/runtime/object.h"
#include "minsk_private/code_analysis/binding/binary_operator_kind.h"

static const MskBoundBinaryOperator kOperators[] = {
    {
        .syntax_kind = kMskSyntaxKindPlusToken,
        .kind = kMskBoundBinaryOperatorKindAddition,
        .left_type = kMskObjectKindInteger,
        .right_type = kMskObjectKindInteger,
        .result_type = kMskObjectKindInteger,
    },
    {
        .syntax_kind = kMskSyntaxKindMinusToken,
        .kind = kMskBoundBinaryOperatorKindSubtraction,
        .left_type = kMskObjectKindInteger,
        .right_type = kMskObjectKindInteger,
        .result_type = kMskObjectKindInteger,
    },
    {
        .syntax_kind = kMskSyntaxKindStarToken,
        .kind = kMskBoundBinaryOperatorKindMultiplication,
        .left_type = kMskObjectKindInteger,
        .right_type = kMskObjectKindInteger,
        .result_type = kMskObjectKindInteger,
    },
    {
        .syntax_kind = kMskSyntaxKindSlashToken,
        .kind = kMskBoundBinaryOperatorKindDivision,
        .left_type = kMskObjectKindInteger,
        .right_type = kMskObjectKindInteger,
        .result_type = kMskObjectKindInteger,
    },
    {
        .syntax_kind = kMskSyntaxKindAmpersandAmpersandToken,
        .kind = kMskBoundBinaryOperatorKindLogicalAnd,
        .left_type = kMskObjectKindBoolean,
        .right_type = kMskObjectKindBoolean,
        .result_type = kMskObjectKindBoolean,
    },
    {
        .syntax_kind = kMskSyntaxKindPipePipeToken,
        .kind = kMskBoundBinaryOperatorKindLogicalOr,
        .left_type = kMskObjectKindBoolean,
        .right_type = kMskObjectKindBoolean,
        .result_type = kMskObjectKindBoolean,
    },
    {
        .syntax_kind = kMskSyntaxKindEqualsEqualsToken,
        .kind = kMskBoundBinaryOperatorKindEquality,
        .left_type = kMskObjectKindInteger,
        .right_type = kMskObjectKindInteger,
        .result_type = kMskObjectKindBoolean,
    },
    {
        .syntax_kind = kMskSyntaxKindEqualsEqualsToken,
        .kind = kMskBoundBinaryOperatorKindEquality,
        .left_type = kMskObjectKindBoolean,
        .right_type = kMskObjectKindBoolean,
        .result_type = kMskObjectKindBoolean,
    },
    {
        .syntax_kind = kMskSyntaxKindBangEqualsToken,
        .kind = kMskBoundBinaryOperatorKindInequality,
        .left_type = kMskObjectKindInteger,
        .right_type = kMskObjectKindInteger,
        .result_type = kMskObjectKindBoolean,
    },
    {
        .syntax_kind = kMskSyntaxKindBangEqualsToken,
        .kind = kMskBoundBinaryOperatorKindInequality,
        .left_type = kMskObjectKindBoolean,
        .right_type = kMskObjectKindBoolean,
        .result_type = kMskObjectKindBoolean,
    },
};

const MskBoundBinaryOperator* MskBindBinaryOperator(
    MskSyntaxKind syntax_kind,
    MskRuntimeObjectKind left_type,
    MskRuntimeObjectKind right_type) {
  for (size_t i = 0; i < sizeof(kOperators) / sizeof(MskBoundBinaryOperator);
       ++i) {
    const MskBoundBinaryOperator* op = &kOperators[i];
    if (op->syntax_kind == syntax_kind && op->left_type == left_type &&
        op->right_type == right_type) {
      return op;
    }
  }
  return NULL;
}

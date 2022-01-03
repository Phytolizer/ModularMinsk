#include "minsk_private/code_analysis/binding/binder.h"

#include <assert.h>

#include "minsk/code_analysis/syntax/binary_expression.h"
#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/code_analysis/syntax/literal_expression.h"
#include "minsk/code_analysis/syntax/parenthesized_expression.h"
#include "minsk/code_analysis/syntax/unary_expression.h"
#include "minsk/runtime/object.h"
#include "minsk_private/code_analysis/binding/binary_expression.h"
#include "minsk_private/code_analysis/binding/binary_operator_kind.h"
#include "minsk_private/code_analysis/binding/expression.h"
#include "minsk_private/code_analysis/binding/literal_expression.h"
#include "minsk_private/code_analysis/binding/node.h"
#include "minsk_private/code_analysis/binding/unary_expression.h"
#include "minsk_private/code_analysis/binding/unary_operator_kind.h"
#include "string/string.h"

static MskBoundExpression* BindLiteralExpression(MskBinder* binder,
                                                 MskExpressionSyntax* syntax);
static MskBoundExpression* BindUnaryExpression(MskBinder* binder,
                                               MskExpressionSyntax* syntax);
static MskBoundExpression* BindBinaryExpression(MskBinder* binder,
                                                MskExpressionSyntax* syntax);
static MskBoundExpression* BindParenthesizedExpression(
    MskBinder* binder,
    MskExpressionSyntax* syntax);

MskBoundExpression* MskBinderBindExpression(MskBinder* binder,
                                            MskExpressionSyntax* syntax) {
  switch (syntax->cls) {
#define X(x)                         \
  case kMskSyntaxExpressionClass##x: \
    return Bind##x##Expression(binder, syntax);
    MSK__EXPRESSION_CLASSES
#undef X
    default:
      assert(false && "corrupt syntax class");
  }
}

void MskBinderFree(MskBinder* binder) {
  MskDiagnosticBagFree(&binder->diagnostics);
}

MskBoundExpression* BindLiteralExpression(MskBinder* binder,
                                          MskExpressionSyntax* syntax) {
  MskLiteralExpressionSyntax* lit = (MskLiteralExpressionSyntax*)syntax;
  return (MskBoundExpression*)MskBoundLiteralExpressionNew(lit->value);
}

MskBoundExpression* BindUnaryExpression(MskBinder* binder,
                                        MskExpressionSyntax* syntax) {
  MskUnaryExpressionSyntax* unary = (MskUnaryExpressionSyntax*)syntax;
  MskBoundExpression* operand = MskBinderBindExpression(binder, unary->operand);
  const MskBoundUnaryOperator* op = MskBindUnaryOperator(
      unary->operator_token.kind, MskBoundExpressionGetType(operand));
  if (op == NULL) {
    VEC_PUSH(&binder->diagnostics,
             StringFormat("Unary operator '%" STRING_FMT
                          "' is not defined for type %" STRING_VIEW_FMT ".",
                          STRING_PRINT(unary->operator_token.text),
                          STRING_VIEW_PRINT(MskRuntimeObjectKindName(
                              MskBoundExpressionGetType(operand)))));
    return operand;
  }
  return (MskBoundExpression*)MskBoundUnaryExpressionNew(*op, operand);
}

MskBoundExpression* BindBinaryExpression(MskBinder* binder,
                                         MskExpressionSyntax* syntax) {
  MskBinaryExpressionSyntax* binary = (MskBinaryExpressionSyntax*)syntax;
  MskBoundExpression* left = MskBinderBindExpression(binder, binary->left);
  MskBoundExpression* right = MskBinderBindExpression(binder, binary->right);
  const MskBoundBinaryOperator* op = MskBindBinaryOperator(
      binary->operator_token.kind, MskBoundExpressionGetType(left),
      MskBoundExpressionGetType(right));
  if (op == NULL) {
    VEC_PUSH(&binder->diagnostics,
             StringFormat("Binary operator '%" STRING_FMT
                          "' is not defined for types %" STRING_VIEW_FMT
                          " and %" STRING_VIEW_FMT ".",
                          STRING_PRINT(binary->operator_token.text),
                          STRING_VIEW_PRINT(MskRuntimeObjectKindName(
                              MskBoundExpressionGetType(left))),
                          STRING_VIEW_PRINT(MskRuntimeObjectKindName(
                              MskBoundExpressionGetType(right)))));
    MskBoundNodeFree(&right->base);
    free(right);
    return left;
  }
  return (MskBoundExpression*)MskBoundBinaryExpressionNew(left, *op, right);
}

MskBoundExpression* BindParenthesizedExpression(MskBinder* binder,
                                                MskExpressionSyntax* syntax) {
  MskParenthesizedExpressionSyntax* paren =
      (MskParenthesizedExpressionSyntax*)syntax;
  return MskBinderBindExpression(binder, paren->expression);
}

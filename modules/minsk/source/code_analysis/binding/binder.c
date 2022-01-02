#include "minsk_private/code_analysis/binding/binder.h"

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

static MskBoundUnaryOperatorKind BindUnaryOperatorKind(
    MskSyntaxKind kind,
    MskRuntimeObjectKind operand_type);
static MskBoundBinaryOperatorKind BindBinaryOperatorKind(
    MskSyntaxKind kind,
    MskRuntimeObjectKind left_type,
    MskRuntimeObjectKind right_type);

MskBoundExpression* MskBinderBindExpression(MskBinder* binder,
                                            MskExpressionSyntax* syntax) {
  switch (syntax->cls) {
#define X(x)                         \
  case kMskSyntaxExpressionClass##x: \
    return Bind##x##Expression(binder, syntax);
    MSK__EXPRESSION_CLASSES
#undef X
    default:
      return NULL;
  }
}

void MskBinderFree(MskBinder* binder) {
  MskDiagnosticsFree(&binder->diagnostics);
}

MskBoundExpression* BindLiteralExpression(MskBinder* binder,
                                          MskExpressionSyntax* syntax) {
  MskLiteralExpressionSyntax* lit = (MskLiteralExpressionSyntax*)syntax;
  return (MskBoundExpression*)MskBoundLiteralExpressionNew(
      lit->literal_token.value);
}

MskBoundExpression* BindUnaryExpression(MskBinder* binder,
                                        MskExpressionSyntax* syntax) {
  MskUnaryExpressionSyntax* unary = (MskUnaryExpressionSyntax*)syntax;
  MskBoundExpression* operand = MskBinderBindExpression(binder, unary->operand);
  if (operand == NULL) {
    return NULL;
  }
  MskBoundUnaryOperatorKind operator_kind = BindUnaryOperatorKind(
      unary->operator_token.kind, MskBoundExpressionGetType(operand));
  if (operator_kind == kMskBoundUnaryOperatorKindInvalid) {
    VEC_PUSH(&binder->diagnostics,
             StringFormat("Unary operator '%" STRING_FMT
                          "' is not defined for type %" STRING_VIEW_FMT ".",
                          STRING_PRINT(unary->operator_token.text),
                          STRING_VIEW_PRINT(MskRuntimeObjectKindName(
                              MskBoundExpressionGetType(operand)))));
    return operand;
  }
  return (MskBoundExpression*)MskBoundUnaryExpressionNew(operator_kind,
                                                         operand);
}

MskBoundExpression* BindBinaryExpression(MskBinder* binder,
                                         MskExpressionSyntax* syntax) {
  MskBinaryExpressionSyntax* binary = (MskBinaryExpressionSyntax*)syntax;
  MskBoundExpression* left = MskBinderBindExpression(binder, binary->left);
  if (left == NULL) {
    return NULL;
  }
  MskBoundExpression* right = MskBinderBindExpression(binder, binary->right);
  if (right == NULL) {
    return NULL;
  }
  MskBoundBinaryOperatorKind operator_kind = BindBinaryOperatorKind(
      binary->operator_token.kind, MskBoundExpressionGetType(left),
      MskBoundExpressionGetType(right));
  if (operator_kind == kMskBoundBinaryOperatorKindInvalid) {
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
  return (MskBoundExpression*)MskBoundBinaryExpressionNew(left, operator_kind,
                                                          right);
}

MskBoundExpression* BindParenthesizedExpression(MskBinder* binder,
                                                MskExpressionSyntax* syntax) {
  MskParenthesizedExpressionSyntax* paren =
      (MskParenthesizedExpressionSyntax*)syntax;
  return MskBinderBindExpression(binder, paren->expression);
}

MskBoundUnaryOperatorKind BindUnaryOperatorKind(
    MskSyntaxKind kind,
    MskRuntimeObjectKind operand_type) {
  if (operand_type != kMskObjectKindInteger) {
    return kMskBoundUnaryOperatorKindInvalid;
  }
  switch (kind) {
    case kMskSyntaxKindPlusToken:
      return kMskBoundUnaryOperatorKindIdentity;
    case kMskSyntaxKindMinusToken:
      return kMskBoundUnaryOperatorKindNegation;
    default:
      return kMskBoundUnaryOperatorKindInvalid;
  }
}

MskBoundBinaryOperatorKind BindBinaryOperatorKind(
    MskSyntaxKind kind,
    MskRuntimeObjectKind left_type,
    MskRuntimeObjectKind right_type) {
  if (left_type == kMskObjectKindInteger &&
      right_type == kMskObjectKindInteger) {
    switch (kind) {
      case kMskSyntaxKindPlusToken:
        return kMskBoundBinaryOperatorKindAddition;
      case kMskSyntaxKindMinusToken:
        return kMskBoundBinaryOperatorKindSubtraction;
      case kMskSyntaxKindStarToken:
        return kMskBoundBinaryOperatorKindMultiplication;
      case kMskSyntaxKindSlashToken:
        return kMskBoundBinaryOperatorKindDivision;
      default:
        return kMskBoundBinaryOperatorKindInvalid;
    }
  }
  return kMskBoundBinaryOperatorKindInvalid;
}

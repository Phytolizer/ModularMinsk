#include "minsk/code_analysis/binding/binder.h"

#include <assert.h>

#include "hash/hash.h"
#include "minsk/code_analysis/symbol_table.h"
#include "minsk/code_analysis/syntax/assignment_expression.h"
#include "minsk/code_analysis/syntax/binary_expression.h"
#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/code_analysis/syntax/literal_expression.h"
#include "minsk/code_analysis/syntax/name_expression.h"
#include "minsk/code_analysis/syntax/parenthesized_expression.h"
#include "minsk/code_analysis/syntax/unary_expression.h"
#include "minsk/code_analysis/text/diagnostic_bag.h"
#include "minsk/runtime/object.h"
#include "minsk/code_analysis/binding/assignment_expression.h"
#include "minsk/code_analysis/binding/binary_expression.h"
#include "minsk/code_analysis/binding/binary_operator_kind.h"
#include "minsk/code_analysis/binding/expression.h"
#include "minsk/code_analysis/binding/literal_expression.h"
#include "minsk/code_analysis/binding/node.h"
#include "minsk/code_analysis/binding/unary_expression.h"
#include "minsk/code_analysis/binding/unary_operator_kind.h"
#include "minsk/code_analysis/binding/variable_expression.h"
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
static MskBoundExpression* BindAssignmentExpression(
    MskBinder* binder,
    MskExpressionSyntax* syntax);
static MskBoundExpression* BindNameExpression(MskBinder* binder,
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
    MskDiagnosticBagReportUndefinedUnaryOperator(
        &binder->diagnostics, MskSyntaxTokenGetSpan(unary->operator_token),
        StringAsView(unary->operator_token.text),
        MskBoundExpressionGetType(operand));
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
    MskDiagnosticBagReportUndefinedBinaryOperator(
        &binder->diagnostics, MskSyntaxTokenGetSpan(binary->operator_token),
        StringAsView(binary->operator_token.text),
        MskBoundExpressionGetType(left), MskBoundExpressionGetType(right));
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

MskBoundExpression* BindAssignmentExpression(MskBinder* binder,
                                             MskExpressionSyntax* syntax) {
  MskAssignmentExpressionSyntax* assignment =
      (MskAssignmentExpressionSyntax*)syntax;
  String name = StringDuplicate(assignment->identifier_token.text);
  MskBoundExpression* bound_expression =
      MskBinderBindExpression(binder, assignment->expression);
  return (MskBoundExpression*)MskBoundAssignmentExpressionNew(name,
                                                              bound_expression);
}

MskBoundExpression* BindNameExpression(MskBinder* binder,
                                       MskExpressionSyntax* syntax) {
  MskNameExpressionSyntax* name = (MskNameExpressionSyntax*)syntax;
  StringView name_text = StringAsView(name->identifier_token.text);
  MskRuntimeObject value;
  if (!MskSymbolTableLookup(binder->symbols, name_text, &value)) {
    MskDiagnosticBagReportUndefinedName(
        &binder->diagnostics, MskSyntaxTokenGetSpan(name->identifier_token),
        name_text);
  }
  MskRuntimeObjectKind kind = value.kind;
  return (MskBoundExpression*)MskBoundVariableExpressionNew(
      StringFromView(name_text), kind);
}

#include "minsk/code_analysis/syntax/node.h"

#include <ansi_esc/ansi_esc.h>
#include <stdint.h>

#include "minsk/code_analysis/syntax/assignment_expression.h"
#include "minsk/code_analysis/syntax/binary_expression.h"
#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/code_analysis/syntax/literal_expression.h"
#include "minsk/code_analysis/syntax/name_expression.h"
#include "minsk/code_analysis/syntax/parenthesized_expression.h"
#include "minsk/code_analysis/syntax/token.h"
#include "minsk/code_analysis/syntax/unary_expression.h"
#include "minsk/runtime/object.h"

const char* const kMskSyntaxClassNames[] = {
#define X(x) [kMskSyntaxNodeClass##x] = #x,
    MSK__SYNTAX_NODE_CLASSES
#undef X
};

StringView MskSyntaxNodeClassName(MskSyntaxNodeClass kind) {
  return StringViewFromC(kMskSyntaxClassNames[kind]);
}

static MskSyntaxKind GetExpressionKind(MskSyntaxNode* node);
static MskSyntaxKind GetTokenKind(MskSyntaxNode* node);

static MskSyntaxNodeChildren GetExpressionChildren(MskSyntaxNode* node);
static MskSyntaxNodeChildren GetTokenChildren(MskSyntaxNode* node);
static MskSyntaxNodeChildren GetBinaryExpressionChildren(
    MskExpressionSyntax* syntax);
static MskSyntaxNodeChildren GetLiteralExpressionChildren(
    MskExpressionSyntax* syntax);
static MskSyntaxNodeChildren GetParenthesizedExpressionChildren(
    MskExpressionSyntax* syntax);
static MskSyntaxNodeChildren GetUnaryExpressionChildren(
    MskExpressionSyntax* syntax);
static MskSyntaxNodeChildren GetNameExpressionChildren(
    MskExpressionSyntax* syntax);
static MskSyntaxNodeChildren GetAssignmentExpressionChildren(
    MskExpressionSyntax* syntax);

static void PrettyPrint(MskSyntaxNode* node,
                        FILE* fp,
                        bool colors,
                        String indent,
                        bool is_last);

MskSyntaxKind MskSyntaxNodeGetKind(MskSyntaxNode* node) {
  switch (node->cls) {
#define X(x)                   \
  case kMskSyntaxNodeClass##x: \
    return Get##x##Kind(node);
    MSK__SYNTAX_NODE_CLASSES
#undef X
    default:
      return kMskSyntaxKindInvalid;
  }
}

MskSyntaxNodeChildren MskSyntaxNodeGetChildren(MskSyntaxNode* node) {
  switch (node->cls) {
#define X(x)                   \
  case kMskSyntaxNodeClass##x: \
    return Get##x##Children(node);
    MSK__SYNTAX_NODE_CLASSES
#undef X
    default:
      return (MskSyntaxNodeChildren)VEC_INIT_DEFAULT(MskSyntaxNode*);
  }
}

void MskSyntaxNodePrettyPrint(MskSyntaxNode* node, FILE* fp, bool colors) {
  PrettyPrint(node, fp, colors, STRING_INIT, true);
}

MskSyntaxKind GetExpressionKind(MskSyntaxNode* node) {
  MskExpressionSyntax* syntax = (MskExpressionSyntax*)node;
  switch (syntax->cls) {
#define X(x)                         \
  case kMskSyntaxExpressionClass##x: \
    return kMskSyntaxKind##x##Expression;
    MSK__EXPRESSION_CLASSES
#undef X
    default:
      return kMskSyntaxKindInvalid;
  }
}

MskSyntaxKind GetTokenKind(MskSyntaxNode* node) {
  MskSyntaxToken* syntax = (MskSyntaxToken*)node;
  return syntax->kind;
}

MskSyntaxNodeChildren GetExpressionChildren(MskSyntaxNode* node) {
  switch (((MskExpressionSyntax*)node)->cls) {
#define X(x)                         \
  case kMskSyntaxExpressionClass##x: \
    return Get##x##ExpressionChildren((MskExpressionSyntax*)node);
    MSK__EXPRESSION_CLASSES
#undef X
    default:
      return (MskSyntaxNodeChildren)VEC_INIT_DEFAULT(MskSyntaxNode*);
  }
}

MskSyntaxNodeChildren GetTokenChildren(MskSyntaxNode* node) {
  (void)node;
  return (MskSyntaxNodeChildren)VEC_INIT_DEFAULT(MskSyntaxNode*);
}

MskSyntaxNodeChildren GetBinaryExpressionChildren(MskExpressionSyntax* syntax) {
  MskBinaryExpressionSyntax* binary = (MskBinaryExpressionSyntax*)syntax;
  MskSyntaxNodeChildren children = VEC_INIT_DEFAULT(MskSyntaxNode*);
  VEC_PUSH(&children, &binary->left->base);
  VEC_PUSH(&children, &binary->operator_token.base);
  VEC_PUSH(&children, &binary->right->base);
  return children;
}

MskSyntaxNodeChildren GetLiteralExpressionChildren(
    MskExpressionSyntax* syntax) {
  MskLiteralExpressionSyntax* literal = (MskLiteralExpressionSyntax*)syntax;
  MskSyntaxNodeChildren children = VEC_INIT_DEFAULT(MskSyntaxNode*);
  VEC_PUSH(&children, &literal->literal_token.base);
  return children;
}

MskSyntaxNodeChildren GetParenthesizedExpressionChildren(
    MskExpressionSyntax* syntax) {
  MskParenthesizedExpressionSyntax* parenthesized =
      (MskParenthesizedExpressionSyntax*)syntax;
  MskSyntaxNodeChildren children = VEC_INIT_DEFAULT(MskSyntaxNode*);
  VEC_PUSH(&children, &parenthesized->open_parenthesis_token.base);
  VEC_PUSH(&children, &parenthesized->expression->base);
  VEC_PUSH(&children, &parenthesized->close_parenthesis_token.base);
  return children;
}

MskSyntaxNodeChildren GetUnaryExpressionChildren(MskExpressionSyntax* syntax) {
  MskUnaryExpressionSyntax* unary = (MskUnaryExpressionSyntax*)syntax;
  MskSyntaxNodeChildren children = VEC_INIT_DEFAULT(MskSyntaxNode*);
  VEC_PUSH(&children, &unary->operator_token.base);
  VEC_PUSH(&children, &unary->operand->base);
  return children;
}

MskSyntaxNodeChildren GetNameExpressionChildren(MskExpressionSyntax* syntax) {
  MskNameExpressionSyntax* name = (MskNameExpressionSyntax*)syntax;
  MskSyntaxNodeChildren children = VEC_INIT_DEFAULT(MskSyntaxNode*);
  VEC_PUSH(&children, &name->identifier_token.base);
  return children;
}

MskSyntaxNodeChildren GetAssignmentExpressionChildren(
    MskExpressionSyntax* syntax) {
  MskAssignmentExpressionSyntax* assignment =
      (MskAssignmentExpressionSyntax*)syntax;
  MskSyntaxNodeChildren children = VEC_INIT_DEFAULT(MskSyntaxNode*);
  VEC_PUSH(&children, &assignment->identifier_token.base);
  VEC_PUSH(&children, &assignment->equals_token.base);
  VEC_PUSH(&children, &assignment->expression->base);
  return children;
}

void PrettyPrint(MskSyntaxNode* node,
                 FILE* fp,
                 bool colors,
                 String indent,
                 bool is_last) {
  if (colors) {
    fprintf(fp, ANSI_ESC_DIM ANSI_ESC_FG_WHITE);
  }
  fprintf(fp, "%" STRING_FMT, STRING_PRINT(indent));
  StringView marker = StringViewFromC(is_last ? "└── " : "├── ");
  fprintf(fp, "%" STRING_VIEW_FMT, STRING_VIEW_PRINT(marker));
  if (colors) {
    fprintf(fp, ANSI_ESC_RESET);
    if (node->cls == kMskSyntaxNodeClassToken) {
      fprintf(fp, ANSI_ESC_FG_CYAN);
    } else {
      fprintf(fp, ANSI_ESC_FG_BLUE);
    }
  }
  fprintf(fp, "%" STRING_VIEW_FMT,
          STRING_VIEW_PRINT(MskSyntaxKindName(MskSyntaxNodeGetKind(node))));
  if (colors) {
    fprintf(fp, ANSI_ESC_RESET);
  }
  if (node->cls == kMskSyntaxNodeClassToken &&
      ((MskSyntaxToken*)node)->value.kind != kMskObjectKindNull) {
    MskSyntaxToken* tok = (MskSyntaxToken*)node;
    if (colors) {
      fprintf(fp, ANSI_ESC_FG_MAGENTA);
    }
    fprintf(fp, " %" STRING_VIEW_FMT "(",
            STRING_VIEW_PRINT(MskRuntimeObjectKindName(tok->value.kind)));
    MskRuntimeObjectPrint(&tok->value, fp);
    fprintf(fp, ")");
  }
  fprintf(fp, "\n");
  if (colors) {
    fprintf(fp, ANSI_ESC_RESET);
  }
  StringAppendC(&indent, is_last ? "    " : "│   ");
  MskSyntaxNodeChildren children = MskSyntaxNodeGetChildren(node);
  for (uint64_t i = 0; i < VEC_SIZE(&children); ++i) {
    PrettyPrint(children.data[i], fp, colors, StringDuplicate(indent),
                i == VEC_SIZE(&children) - 1);
  }
  StringFree(&indent);
}

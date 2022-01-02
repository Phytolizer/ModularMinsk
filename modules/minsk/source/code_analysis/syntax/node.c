#include "minsk/code_analysis/syntax/node.h"

#include <ansi_esc/ansi_esc.h>
#include <stdint.h>

#include "minsk/code_analysis/syntax/binary_expression.h"
#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/code_analysis/syntax/literal_expression.h"
#include "minsk/code_analysis/syntax/token.h"
#include "minsk/runtime/object.h"

const char* const kMskSyntaxKindNames[] = {
#define X(x) [kMskSyntaxNodeKind##x] = #x,
    MSK__SYNTAX_NODES
#undef X
};

StringView MskSyntaxNodeKindName(MskSyntaxNodeKind kind) {
  return StringViewFromC(kMskSyntaxKindNames[kind]);
}

static MskSyntaxKind GetExpressionKind(MskSyntaxNode* node);
static MskSyntaxKind GetTokenKind(MskSyntaxNode* node);
static MskSyntaxKind GetBinaryExpressionKind(MskExpressionSyntax* syntax);
static MskSyntaxKind GetLiteralExpressionKind(MskExpressionSyntax* syntax);

static MskSyntaxNodeChildren GetExpressionChildren(MskSyntaxNode* node);
static MskSyntaxNodeChildren GetTokenChildren(MskSyntaxNode* node);
static MskSyntaxNodeChildren GetBinaryExpressionChildren(
    MskExpressionSyntax* syntax);
static MskSyntaxNodeChildren GetLiteralExpressionChildren(
    MskExpressionSyntax* syntax);

static void PrettyPrint(MskSyntaxNode* node,
                        FILE* fp,
                        bool colors,
                        String indent,
                        bool is_last);

MskSyntaxKind MskSyntaxNodeGetKind(MskSyntaxNode* node) {
  switch (node->kind) {
#define X(x)                  \
  case kMskSyntaxNodeKind##x: \
    return Get##x##Kind(node);
    MSK__SYNTAX_NODES
#undef X
    default:
      return kMskSyntaxKindInvalid;
  }
}

MskSyntaxNodeChildren MskSyntaxNodeGetChildren(MskSyntaxNode* node) {
  switch (node->kind) {
#define X(x)                  \
  case kMskSyntaxNodeKind##x: \
    return Get##x##Children(node);
    MSK__SYNTAX_NODES
#undef X
    default:
      return (MskSyntaxNodeChildren){0};
  }
}

void MskSyntaxNodePrettyPrint(MskSyntaxNode* node, FILE* fp, bool colors) {
  PrettyPrint(node, fp, colors, (String){0}, true);
}

MskSyntaxKind GetExpressionKind(MskSyntaxNode* node) {
  MskExpressionSyntax* syntax = (MskExpressionSyntax*)node;
  switch (syntax->kind) {
#define X(x)                        \
  case kMskSyntaxExpressionKind##x: \
    return Get##x##ExpressionKind(syntax);
    MSK__EXPRESSION_KINDS
#undef X
    default:
      return kMskSyntaxKindInvalid;
  }
}

MskSyntaxKind GetTokenKind(MskSyntaxNode* node) {
  MskSyntaxToken* syntax = (MskSyntaxToken*)node;
  return syntax->kind;
}

MskSyntaxKind GetBinaryExpressionKind(MskExpressionSyntax* syntax) {
  (void)syntax;
  return kMskSyntaxKindBinaryExpression;
}

MskSyntaxKind GetLiteralExpressionKind(MskExpressionSyntax* syntax) {
  (void)syntax;
  return kMskSyntaxKindLiteralExpression;
}

MskSyntaxNodeChildren GetExpressionChildren(MskSyntaxNode* node) {
  switch (((MskExpressionSyntax*)node)->kind) {
#define X(x)                        \
  case kMskSyntaxExpressionKind##x: \
    return Get##x##ExpressionChildren((MskExpressionSyntax*)node);
    MSK__EXPRESSION_KINDS
#undef X
    default:
      return (MskSyntaxNodeChildren){0};
  }
}

MskSyntaxNodeChildren GetTokenChildren(MskSyntaxNode* node) {
  (void)node;
  return (MskSyntaxNodeChildren){0};
}

MskSyntaxNodeChildren GetBinaryExpressionChildren(MskExpressionSyntax* syntax) {
  MskBinaryExpressionSyntax* binary = (MskBinaryExpressionSyntax*)syntax;
  MskSyntaxNodeChildren children = {0};
  VEC_PUSH(&children, &binary->left->base);
  VEC_PUSH(&children, &binary->operator_token.base);
  VEC_PUSH(&children, &binary->right->base);
  return children;
}

MskSyntaxNodeChildren GetLiteralExpressionChildren(
    MskExpressionSyntax* syntax) {
  MskLiteralExpressionSyntax* literal = (MskLiteralExpressionSyntax*)syntax;
  MskSyntaxNodeChildren children = {0};
  VEC_PUSH(&children, &literal->literal_token.base);
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
    if (node->kind == kMskSyntaxNodeKindToken) {
      fprintf(fp, ANSI_ESC_FG_BLUE);
    } else {
      fprintf(fp, ANSI_ESC_FG_GREEN);
    }
  }
  fprintf(fp, "%" STRING_VIEW_FMT,
          STRING_VIEW_PRINT(MskSyntaxKindName(MskSyntaxNodeGetKind(node))));
  if (colors) {
    fprintf(fp, ANSI_ESC_RESET);
  }
  if (node->kind == kMskSyntaxNodeKindToken &&
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
  for (uint64_t i = 0; i < children.size; ++i) {
    PrettyPrint(children.data[i], fp, colors, indent, i == children.size - 1);
  }
}

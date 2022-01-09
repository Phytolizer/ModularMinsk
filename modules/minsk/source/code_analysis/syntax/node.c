#include "minsk/code_analysis/syntax/node.h"

#include <ansi_esc/ansi_esc.h>
#include <phyto/string/string.h>
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

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_IMPL(MskSyntaxNodeChildren, MskSyntaxNode*);

static const MskSyntaxNodeChildren_callbacks_t kMskSyntaxNodeChildrenCallbacks =
    {0};

phyto_string_span_t MskSyntaxNodeClassName(MskSyntaxNodeClass kind) {
  return phyto_string_span_from_c(kMskSyntaxClassNames[kind]);
}

static MskSyntaxKind GetExpressionKind(MskSyntaxNode* node);
static MskSyntaxKind GetTokenKind(MskSyntaxNode* node);

static MskSyntaxNodeChildren_t GetExpressionChildren(MskSyntaxNode* node);
static MskSyntaxNodeChildren_t GetTokenChildren(MskSyntaxNode* node);
static MskSyntaxNodeChildren_t GetBinaryExpressionChildren(
    MskExpressionSyntax* syntax);
static MskSyntaxNodeChildren_t GetLiteralExpressionChildren(
    MskExpressionSyntax* syntax);
static MskSyntaxNodeChildren_t GetParenthesizedExpressionChildren(
    MskExpressionSyntax* syntax);
static MskSyntaxNodeChildren_t GetUnaryExpressionChildren(
    MskExpressionSyntax* syntax);
static MskSyntaxNodeChildren_t GetNameExpressionChildren(
    MskExpressionSyntax* syntax);
static MskSyntaxNodeChildren_t GetAssignmentExpressionChildren(
    MskExpressionSyntax* syntax);

static void PrettyPrint(MskSyntaxNode* node,
                        FILE* fp,
                        bool colors,
                        phyto_string_t indent,
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

MskSyntaxNodeChildren_t MskSyntaxNodeGetChildren(MskSyntaxNode* node) {
  switch (node->cls) {
#define X(x)                   \
  case kMskSyntaxNodeClass##x: \
    return Get##x##Children(node);
    MSK__SYNTAX_NODE_CLASSES
#undef X
    default:
      return MskSyntaxNodeChildren_init(&kMskSyntaxNodeChildrenCallbacks);
  }
}

void MskSyntaxNodePrettyPrint(MskSyntaxNode* node, FILE* fp, bool colors) {
  PrettyPrint(node, fp, colors, phyto_string_new(), true);
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

MskSyntaxNodeChildren_t GetExpressionChildren(MskSyntaxNode* node) {
  switch (((MskExpressionSyntax*)node)->cls) {
#define X(x)                         \
  case kMskSyntaxExpressionClass##x: \
    return Get##x##ExpressionChildren((MskExpressionSyntax*)node);
    MSK__EXPRESSION_CLASSES
#undef X
    default:
      return MskSyntaxNodeChildren_init(&kMskSyntaxNodeChildrenCallbacks);
  }
}

MskSyntaxNodeChildren_t GetTokenChildren(MskSyntaxNode* node) {
  (void)node;
  return MskSyntaxNodeChildren_init(&kMskSyntaxNodeChildrenCallbacks);
}

MskSyntaxNodeChildren_t GetBinaryExpressionChildren(
    MskExpressionSyntax* syntax) {
  MskBinaryExpressionSyntax* binary = (MskBinaryExpressionSyntax*)syntax;
  MskSyntaxNodeChildren_t children =
      MskSyntaxNodeChildren_init(&kMskSyntaxNodeChildrenCallbacks);
  MskSyntaxNodeChildren_append(&children, &binary->left->base);
  MskSyntaxNodeChildren_append(&children, &binary->operator_token.base);
  MskSyntaxNodeChildren_append(&children, &binary->right->base);
  return children;
}

MskSyntaxNodeChildren_t GetLiteralExpressionChildren(
    MskExpressionSyntax* syntax) {
  MskLiteralExpressionSyntax* literal = (MskLiteralExpressionSyntax*)syntax;
  MskSyntaxNodeChildren_t children =
      MskSyntaxNodeChildren_init(&kMskSyntaxNodeChildrenCallbacks);
  MskSyntaxNodeChildren_append(&children, &literal->literal_token.base);
  return children;
}

MskSyntaxNodeChildren_t GetParenthesizedExpressionChildren(
    MskExpressionSyntax* syntax) {
  MskParenthesizedExpressionSyntax* parenthesized =
      (MskParenthesizedExpressionSyntax*)syntax;
  MskSyntaxNodeChildren_t children =
      MskSyntaxNodeChildren_init(&kMskSyntaxNodeChildrenCallbacks);
  MskSyntaxNodeChildren_append(&children,
                               &parenthesized->open_parenthesis_token.base);
  MskSyntaxNodeChildren_append(&children, &parenthesized->expression->base);
  MskSyntaxNodeChildren_append(&children,
                               &parenthesized->close_parenthesis_token.base);
  return children;
}

MskSyntaxNodeChildren_t GetUnaryExpressionChildren(
    MskExpressionSyntax* syntax) {
  MskUnaryExpressionSyntax* unary = (MskUnaryExpressionSyntax*)syntax;
  MskSyntaxNodeChildren_t children =
      MskSyntaxNodeChildren_init(&kMskSyntaxNodeChildrenCallbacks);
  MskSyntaxNodeChildren_append(&children, &unary->operator_token.base);
  MskSyntaxNodeChildren_append(&children, &unary->operand->base);
  return children;
}

MskSyntaxNodeChildren_t GetNameExpressionChildren(MskExpressionSyntax* syntax) {
  MskNameExpressionSyntax* name = (MskNameExpressionSyntax*)syntax;
  MskSyntaxNodeChildren_t children =
      MskSyntaxNodeChildren_init(&kMskSyntaxNodeChildrenCallbacks);
  MskSyntaxNodeChildren_append(&children, &name->identifier_token.base);
  return children;
}

MskSyntaxNodeChildren_t GetAssignmentExpressionChildren(
    MskExpressionSyntax* syntax) {
  MskAssignmentExpressionSyntax* assignment =
      (MskAssignmentExpressionSyntax*)syntax;
  MskSyntaxNodeChildren_t children =
      MskSyntaxNodeChildren_init(&kMskSyntaxNodeChildrenCallbacks);
  MskSyntaxNodeChildren_append(&children, &assignment->identifier_token.base);
  MskSyntaxNodeChildren_append(&children, &assignment->equals_token.base);
  MskSyntaxNodeChildren_append(&children, &assignment->expression->base);
  return children;
}

void PrettyPrint(MskSyntaxNode* node,
                 FILE* fp,
                 bool colors,
                 phyto_string_t indent,
                 bool is_last) {
  if (colors) {
    fprintf(fp, ANSI_ESC_DIM ANSI_ESC_FG_WHITE);
  }
  fprintf(fp, "%" PHYTO_STRING_FORMAT, PHYTO_STRING_PRINTF_ARGS(indent));
  phyto_string_span_t marker =
      phyto_string_span_from_c(is_last ? "└── " : "├── ");
  fprintf(fp, "%" PHYTO_STRING_FORMAT, PHYTO_STRING_VIEW_PRINTF_ARGS(marker));
  if (colors) {
    fprintf(fp, ANSI_ESC_RESET);
    if (node->cls == kMskSyntaxNodeClassToken) {
      fprintf(fp, ANSI_ESC_FG_CYAN);
    } else {
      fprintf(fp, ANSI_ESC_FG_BLUE);
    }
  }
  fprintf(fp, "%" PHYTO_STRING_FORMAT,
          PHYTO_STRING_VIEW_PRINTF_ARGS(
              MskSyntaxKindName(MskSyntaxNodeGetKind(node))));
  if (colors) {
    fprintf(fp, ANSI_ESC_RESET);
  }
  if (node->cls == kMskSyntaxNodeClassToken &&
      ((MskSyntaxToken*)node)->value.kind != kMskObjectKindNull) {
    MskSyntaxToken* tok = (MskSyntaxToken*)node;
    if (colors) {
      fprintf(fp, ANSI_ESC_FG_MAGENTA);
    }
    fprintf(fp, " %" PHYTO_STRING_FORMAT "(",
            PHYTO_STRING_VIEW_PRINTF_ARGS(
                MskRuntimeObjectKindName(tok->value.kind)));
    MskRuntimeObjectPrint(&tok->value, fp);
    fprintf(fp, ")");
  }
  fprintf(fp, "\n");
  if (colors) {
    fprintf(fp, ANSI_ESC_RESET);
  }
  phyto_string_append_c(&indent, is_last ? "    " : "│   ");
  MskSyntaxNodeChildren_t children = MskSyntaxNodeGetChildren(node);
  for (uint64_t i = 0; i < children.size; ++i) {
    PrettyPrint(children.data[i], fp, colors, phyto_string_copy(indent),
                i == children.size - 1);
  }
  phyto_string_free(&indent);
}

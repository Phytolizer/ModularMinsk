#include "minsk_private/code_analysis/syntax/parser.h"

#include <stdint.h>
#include <stdlib.h>

#include "minsk/code_analysis/syntax/binary_expression.h"
#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/code_analysis/syntax/literal_expression.h"
#include "minsk/code_analysis/syntax/node.h"
#include "minsk/code_analysis/syntax/parenthesized_expression.h"
#include "minsk/code_analysis/syntax/token.h"
#include "minsk/code_analysis/syntax/unary_expression.h"
#include "minsk/runtime/object.h"
#include "minsk_private/code_analysis/syntax/facts.h"
#include "minsk_private/code_analysis/syntax/lexer.h"
#include "string/string.h"

static MskSyntaxToken* Peek(MskSyntaxParser* parser, int64_t offset);
static MskSyntaxToken* Current(MskSyntaxParser* parser);
static MskSyntaxToken NextToken(MskSyntaxParser* parser);
static MskSyntaxToken MatchToken(MskSyntaxParser* parser, MskSyntaxKind kind);
static MskExpressionSyntax* ParseExpression(MskSyntaxParser* parser);
static MskExpressionSyntax* ParseBinaryExpression(MskSyntaxParser* parser,
                                                  uint64_t parent_precedence);
static MskExpressionSyntax* ParsePrimaryExpression(MskSyntaxParser* parser);

MskSyntaxParser MskSyntaxParserNew(StringView text) {
  MskSyntaxLexer lexer = MskNewSyntaxLexer(text);
  MskSyntaxParser parser = {0};
  while (true) {
    MskSyntaxToken token = MskSyntaxLexerLex(&lexer);
    if (token.kind != kMskSyntaxKindBadToken &&
        token.kind != kMskSyntaxKindWhitespaceToken) {
      VEC_PUSH(&parser.tokens, token);
    }
    if (token.kind == kMskSyntaxKindEndOfFileToken) {
      break;
    }
  }
  VEC_APPEND(&parser.diagnostics, lexer.diagnostics.data,
             lexer.diagnostics.size);
  return parser;
}

void MskSyntaxParserFree(MskSyntaxParser* parser) {
  for (size_t i = 0; i < parser->tokens.size; ++i) {
    MskSyntaxTokenFree(&parser->tokens.data[i]);
  }
  VEC_FREE(&parser->tokens);
}

MskSyntaxTree MskSyntaxParserParse(MskSyntaxParser* parser) {
  MskExpressionSyntax* root = ParseExpression(parser);
  MskSyntaxToken end_of_file_token =
      MatchToken(parser, kMskSyntaxKindEndOfFileToken);
  return (MskSyntaxTree){
      .root = root,
      .end_of_file_token = end_of_file_token,
      .diagnostics = parser->diagnostics,
  };
}

MskSyntaxToken* Peek(MskSyntaxParser* parser, int64_t offset) {
  if (parser->position + offset >= parser->tokens.size) {
    // Return the last token, this makes the parser see an infinite stream of
    // EOF tokens.
    return &parser->tokens.data[parser->tokens.size - 1];
  }
  if (offset > parser->position) {
    // Return NULL, as we want to crash if the caller tries to peek past the
    // beginning of the stream.
    return NULL;
  }
  return &parser->tokens.data[parser->position + offset];
}

MskSyntaxToken* Current(MskSyntaxParser* parser) {
  return Peek(parser, 0);
}

MskSyntaxToken NextToken(MskSyntaxParser* parser) {
  MskSyntaxToken token = *Current(parser);
  ++parser->position;
  return MskSyntaxTokenDuplicate(token);
}

MskSyntaxToken MatchToken(MskSyntaxParser* parser, MskSyntaxKind kind) {
  if (Current(parser)->kind == kind) {
    return NextToken(parser);
  }
  VEC_PUSH(&parser->diagnostics,
           StringFormat(
               "expected next token to be %" STRING_VIEW_FMT
               ", got %" STRING_VIEW_FMT ".",
               STRING_VIEW_PRINT(MskSyntaxKindName(kind)),
               STRING_VIEW_PRINT(MskSyntaxKindName(Current(parser)->kind))));
  return (MskSyntaxToken){
      .base = {.kind = kMskSyntaxNodeKindToken},
      .kind = kind,
      .position = Current(parser)->position,
      .text = {0},
      .value = MSK_RUNTIME_OBJECT_NULL,
  };
}

MskExpressionSyntax* ParseExpression(MskSyntaxParser* parser) {
  return ParseBinaryExpression(parser, 0);
}

MskExpressionSyntax* ParseBinaryExpression(MskSyntaxParser* parser,
                                           uint64_t parent_precedence) {
  MskExpressionSyntax* left;
  uint64_t unary_operator_precedence =
      MskSyntaxFactsUnaryOperatorPrecedence(Current(parser)->kind);
  if (unary_operator_precedence != 0 &&
      unary_operator_precedence >= parent_precedence) {
    MskSyntaxToken operator_token = MatchToken(parser, Current(parser)->kind);
    MskExpressionSyntax* operand =
        ParseBinaryExpression(parser, unary_operator_precedence);
    left = (MskExpressionSyntax*)MskUnaryExpressionSyntaxNew(operator_token,
                                                             operand);
  } else {
    left = ParsePrimaryExpression(parser);
  }
  while (true) {
    uint64_t precedence =
        MskSyntaxFactsBinaryOperatorPrecedence(Current(parser)->kind);
    if (precedence == 0 || precedence <= parent_precedence) {
      break;
    }
    MskSyntaxToken operator_token = NextToken(parser);
    MskExpressionSyntax* right = ParseBinaryExpression(parser, precedence);
    left = (MskExpressionSyntax*)MskBinaryExpressionSyntaxNew(
        left, operator_token, right);
  }
  return left;
}

MskExpressionSyntax* ParsePrimaryExpression(MskSyntaxParser* parser) {
  if (Current(parser)->kind == kMskSyntaxKindOpenParenthesisToken) {
    MskSyntaxToken open_parenthesis_token =
        MatchToken(parser, kMskSyntaxKindOpenParenthesisToken);
    MskExpressionSyntax* expression = ParseBinaryExpression(parser, 0);
    MskSyntaxToken close_parenthesis_token =
        MatchToken(parser, kMskSyntaxKindCloseParenthesisToken);
    return (MskExpressionSyntax*)MskParenthesizedExpressionSyntaxNew(
        open_parenthesis_token, expression, close_parenthesis_token);
  }
  MskSyntaxToken number_token = MatchToken(parser, kMskSyntaxKindNumberToken);
  return (MskExpressionSyntax*)MskLiteralExpressionSyntaxNew(number_token);
}

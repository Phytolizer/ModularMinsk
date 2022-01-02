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
#include "minsk/runtime/object.h"
#include "minsk_private/code_analysis/syntax/lexer.h"
#include "string/string.h"

static MskSyntaxToken* Peek(MskSyntaxParser* parser, int64_t offset);
static MskSyntaxToken* Current(MskSyntaxParser* parser);
static MskSyntaxToken NextToken(MskSyntaxParser* parser);
static MskSyntaxToken MatchToken(MskSyntaxParser* parser, MskSyntaxKind kind);
static MskExpressionSyntax* ParseBinaryExpression(MskSyntaxParser* parser);
static MskExpressionSyntax* ParseTerm(MskSyntaxParser* parser);
static MskExpressionSyntax* ParseFactor(MskSyntaxParser* parser);
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
  MskExpressionSyntax* root = ParseBinaryExpression(parser);
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
  return token;
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

MskExpressionSyntax* ParseBinaryExpression(MskSyntaxParser* parser) {
  return ParseTerm(parser);
}

MskExpressionSyntax* ParseTerm(MskSyntaxParser* parser) {
  MskExpressionSyntax* left = ParseFactor(parser);

  while (Current(parser)->kind == kMskSyntaxKindPlusToken ||
         Current(parser)->kind == kMskSyntaxKindMinusToken) {
    MskSyntaxToken operator_token = MskSyntaxTokenDuplicate(NextToken(parser));
    MskExpressionSyntax* right = ParseFactor(parser);
    left = (MskExpressionSyntax*)MskBinaryExpressionSyntaxNew(
        left, operator_token, right);
  }

  return left;
}

MskExpressionSyntax* ParseFactor(MskSyntaxParser* parser) {
  MskExpressionSyntax* left = ParsePrimaryExpression(parser);

  while (Current(parser)->kind == kMskSyntaxKindStarToken ||
         Current(parser)->kind == kMskSyntaxKindSlashToken) {
    MskSyntaxToken operator_token = MskSyntaxTokenDuplicate(NextToken(parser));
    MskExpressionSyntax* right = ParsePrimaryExpression(parser);
    left = (MskExpressionSyntax*)MskBinaryExpressionSyntaxNew(
        left, operator_token, right);
  }

  return left;
}

MskExpressionSyntax* ParsePrimaryExpression(MskSyntaxParser* parser) {
  if (Current(parser)->kind == kMskSyntaxKindOpenParenthesisToken) {
    MskSyntaxToken open_parenthesis_token =
        MatchToken(parser, kMskSyntaxKindOpenParenthesisToken);
    MskExpressionSyntax* expression = ParseBinaryExpression(parser);
    MskSyntaxToken close_parenthesis_token =
        MatchToken(parser, kMskSyntaxKindCloseParenthesisToken);
    return (MskExpressionSyntax*)MskParenthesizedExpressionSyntaxNew(
        open_parenthesis_token, expression, close_parenthesis_token);
  }
  MskSyntaxToken number_token =
      MskSyntaxTokenDuplicate(MatchToken(parser, kMskSyntaxKindNumberToken));
  return (MskExpressionSyntax*)MskLiteralExpressionSyntaxNew(number_token);
}

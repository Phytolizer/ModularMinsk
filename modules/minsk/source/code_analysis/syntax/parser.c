#include "minsk/code_analysis/syntax/parser.h"

#include <phyto/string/string.h>
#include <stdint.h>
#include <stdlib.h>

#include "minsk/code_analysis/syntax/assignment_expression.h"
#include "minsk/code_analysis/syntax/binary_expression.h"
#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/facts.h"
#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/code_analysis/syntax/lexer.h"
#include "minsk/code_analysis/syntax/literal_expression.h"
#include "minsk/code_analysis/syntax/name_expression.h"
#include "minsk/code_analysis/syntax/node.h"
#include "minsk/code_analysis/syntax/parenthesized_expression.h"
#include "minsk/code_analysis/syntax/token.h"
#include "minsk/code_analysis/syntax/unary_expression.h"
#include "minsk/code_analysis/text/diagnostic_bag.h"
#include "minsk/runtime/object.h"

static MskSyntaxToken* Peek(MskSyntaxParser* parser, int64_t offset);
static MskSyntaxToken* Current(MskSyntaxParser* parser);
static MskSyntaxToken NextToken(MskSyntaxParser* parser);
static MskSyntaxToken MatchToken(MskSyntaxParser* parser, MskSyntaxKind kind);
static MskExpressionSyntax* ParseExpression(MskSyntaxParser* parser);
static MskExpressionSyntax* ParseAssignmentExpression(MskSyntaxParser* parser);
static MskExpressionSyntax* ParseBinaryExpression(MskSyntaxParser* parser,
                                                  uint64_t parent_precedence);
static MskExpressionSyntax* ParsePrimaryExpression(MskSyntaxParser* parser);

MskSyntaxParser MskSyntaxParserNew(phyto_string_span_t text) {
  MskSyntaxLexer lexer = MskNewSyntaxLexer(text);
  MskSyntaxParser parser = MSK_SYNTAX_PARSER_INIT;
  while (true) {
    MskSyntaxToken token = MskSyntaxLexerLex(&lexer);
    if (token.kind != kMskSyntaxKindBadToken &&
        token.kind != kMskSyntaxKindWhitespaceToken) {
      MskSyntaxTokens_append(&parser.tokens, token);
    } else {
      MskSyntaxTokenFree(&token);
    }
    if (token.kind == kMskSyntaxKindEndOfFileToken) {
      break;
    }
  }
  MskDiagnosticBag_extend(&parser.diagnostics,
                          MskDiagnosticBag_as_span(lexer.diagnostics));
  return parser;
}

void MskSyntaxParserFree(MskSyntaxParser* parser) {
  MskSyntaxTokens_free(&parser->tokens);
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
  if (offset < -(int64_t)parser->position) {
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
  MskDiagnosticBagReportUnexpectedToken(&parser->diagnostics,
                                        MskSyntaxTokenGetSpan(*Current(parser)),
                                        kind, Current(parser)->kind);
  return (MskSyntaxToken){
      .base = {.cls = kMskSyntaxNodeClassToken},
      .kind = kind,
      .position = Current(parser)->position,
      .text = phyto_string_new(),
      .value = MSK_RUNTIME_OBJECT_NULL,
  };
}

MskExpressionSyntax* ParseExpression(MskSyntaxParser* parser) {
  return ParseAssignmentExpression(parser);
}

MskExpressionSyntax* ParseAssignmentExpression(MskSyntaxParser* parser) {
  if (Current(parser)->kind == kMskSyntaxKindIdentifierToken &&
      Peek(parser, 1)->kind == kMskSyntaxKindEqualsToken) {
    MskSyntaxToken identifier_token =
        MatchToken(parser, kMskSyntaxKindIdentifierToken);
    MskSyntaxToken equals_token = MatchToken(parser, kMskSyntaxKindEqualsToken);
    MskExpressionSyntax* right = ParseAssignmentExpression(parser);
    return (MskExpressionSyntax*)MskAssignmentExpressionSyntaxNew(
        identifier_token, equals_token, right);
  }
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
  if (Current(parser)->kind == kMskSyntaxKindTrueKeyword ||
      Current(parser)->kind == kMskSyntaxKindFalseKeyword) {
    MskSyntaxToken keyword_token = NextToken(parser);
    bool value = keyword_token.kind == kMskSyntaxKindTrueKeyword;
    return (MskExpressionSyntax*)MskLiteralExpressionSyntaxNew(
        keyword_token, MskRuntimeObjectNewBoolean(value));
  }
  if (Current(parser)->kind == kMskSyntaxKindIdentifierToken) {
    MskSyntaxToken identifier_token = NextToken(parser);
    return (MskExpressionSyntax*)MskNameExpressionSyntaxNew(identifier_token);
  }
  MskSyntaxToken number_token = MatchToken(parser, kMskSyntaxKindNumberToken);
  return (MskExpressionSyntax*)MskLiteralExpressionSyntaxNew(
      number_token, MSK_RUNTIME_OBJECT_NULL);
}

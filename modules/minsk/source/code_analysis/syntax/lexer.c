#include "minsk/code_analysis/syntax/lexer.h"

#include <phyto/string/string.h>
#include <stdint.h>

#include "minsk/code_analysis/syntax/facts.h"
#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/code_analysis/syntax/node.h"
#include "minsk/code_analysis/text/diagnostic_bag.h"
#include "minsk/runtime/object.h"

static char Look(MskSyntaxLexer* lexer, uint64_t offset);
static char Cur(MskSyntaxLexer* lexer);
static char Peek(MskSyntaxLexer* lexer);
static bool IsLetter(char c);
static bool IsLetterOrDigit(char c);

MskSyntaxLexer MskNewSyntaxLexer(phyto_string_span_t text) {
  return (MskSyntaxLexer){
      .text = text,
      .position = 0,
  };
}

MskSyntaxToken MskSyntaxLexerLex(MskSyntaxLexer* lexer) {
  MskSyntaxKind kind = kMskSyntaxKindBadToken;
  phyto_string_t text = phyto_string_new();
  uint64_t position = lexer->position;
  MskRuntimeObject value = MSK_RUNTIME_OBJECT_NULL;

  switch (Cur(lexer)) {
    case '\0':
      kind = kMskSyntaxKindEndOfFileToken;
      break;
    case ' ':
    case '\t':
    case '\n':
    case '\r':
      while (Cur(lexer) == ' ' || Cur(lexer) == '\t' || Cur(lexer) == '\n' ||
             Cur(lexer) == '\r') {
        lexer->position++;
      }
      kind = kMskSyntaxKindWhitespaceToken;
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
      kind = kMskSyntaxKindNumberToken;
      while (Cur(lexer) >= '0' && Cur(lexer) <= '9') {
        lexer->position++;
      }
      // Create the text early. It's needed for StringViewToI64.
      text = phyto_string_own(
          phyto_string_span_subspan(lexer->text, position, lexer->position));
      phyto_string_conversion_result_i64_t result =
          phyto_string_to_i64(phyto_string_as_span(text));
      if (!result.success) {
        MskDiagnosticBagReportInvalidNumber(
            &lexer->diagnostics, MskTextSpanNew(position, lexer->position),
            phyto_string_as_span(text), phyto_string_span_from_c("int64_t"));
      }
      value = MskRuntimeObjectNewInteger(result.value);
    } break;
    case '+':
      kind = kMskSyntaxKindPlusToken;
      lexer->position++;
      break;
    case '-':
      kind = kMskSyntaxKindMinusToken;
      lexer->position++;
      break;
    case '*':
      kind = kMskSyntaxKindStarToken;
      lexer->position++;
      break;
    case '/':
      kind = kMskSyntaxKindSlashToken;
      lexer->position++;
      break;
    case '(':
      kind = kMskSyntaxKindOpenParenthesisToken;
      lexer->position++;
      break;
    case ')':
      kind = kMskSyntaxKindCloseParenthesisToken;
      lexer->position++;
      break;
    case '!':
      if (Peek(lexer) == '=') {
        kind = kMskSyntaxKindBangEqualsToken;
        lexer->position += 2;
      } else {
        kind = kMskSyntaxKindBangToken;
        lexer->position++;
      }
      break;
    case '&':
      if (Peek(lexer) == '&') {
        kind = kMskSyntaxKindAmpersandAmpersandToken;
        lexer->position += 2;
      }
      break;
    case '|':
      if (Peek(lexer) == '|') {
        kind = kMskSyntaxKindPipePipeToken;
        lexer->position += 2;
      }
      break;
    case '=':
      if (Peek(lexer) == '=') {
        kind = kMskSyntaxKindEqualsEqualsToken;
        lexer->position += 2;
      } else {
        kind = kMskSyntaxKindEqualsToken;
        lexer->position++;
      }
      break;
    default:
      if (IsLetter(Cur(lexer))) {
        while (IsLetterOrDigit(Cur(lexer))) {
          lexer->position++;
        }
        text = phyto_string_own(
            phyto_string_span_subspan(lexer->text, position, lexer->position));
        kind = MskSyntaxFactsKeywordKind(phyto_string_as_span(text));
      }
      break;
  }

  if (kind == kMskSyntaxKindBadToken) {
    MskDiagnosticBagReportBadCharacter(&lexer->diagnostics, position,
                                       Cur(lexer));
    lexer->position++;
  }

  // The text could have been created already.
  if (text.size == 0) {
    text = phyto_string_own(
        phyto_string_span_subspan(lexer->text, position, lexer->position));
  }

  return (MskSyntaxToken){
      .base = {.cls = kMskSyntaxNodeClassToken},
      .kind = kind,
      .text = text,
      .position = position,
      .value = value,
  };
}

char Look(MskSyntaxLexer* lexer, uint64_t offset) {
  if (lexer->position + offset >= lexer->text.size) {
    return '\0';
  }
  return lexer->text.begin[lexer->position + offset];
}

char Cur(MskSyntaxLexer* lexer) {
  return Look(lexer, 0);
}

char Peek(MskSyntaxLexer* lexer) {
  return Look(lexer, 1);
}

bool IsLetter(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool IsLetterOrDigit(char c) {
  return IsLetter(c) || (c >= '0' && c <= '9');
}

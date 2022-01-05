#include "minsk/code_analysis/syntax/lexer.h"

#include <stdint.h>

#include "minsk/code_analysis/syntax/facts.h"
#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/code_analysis/syntax/node.h"
#include "minsk/code_analysis/text/diagnostic_bag.h"
#include "minsk/runtime/object.h"
#include "string/string.h"

static char Look(MskSyntaxLexer* lexer, uint64_t offset);
static char Cur(MskSyntaxLexer* lexer);
static char Peek(MskSyntaxLexer* lexer);
static bool IsLetter(char c);
static bool IsLetterOrDigit(char c);

MskSyntaxLexer MskNewSyntaxLexer(StringView text) {
  return (MskSyntaxLexer){
      .text = text,
      .position = 0,
  };
}

MskSyntaxToken MskSyntaxLexerLex(MskSyntaxLexer* lexer) {
  MskSyntaxKind kind = kMskSyntaxKindBadToken;
  String text = STRING_INIT;
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
      text = StringFromView(
          StringViewSubstring(lexer->text, position, lexer->position));
      StringConversionResultI64 result = StringViewToI64(StringAsView(text));
      if (!result.success) {
        MskDiagnosticBagReportInvalidNumber(
            &lexer->diagnostics, MskTextSpanNew(position, lexer->position),
            StringAsView(text), StringViewFromC("int64_t"));
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
        text = StringFromView(
            StringViewSubstring(lexer->text, position, lexer->position));
        kind = MskSyntaxFactsKeywordKind(StringAsView(text));
      }
      break;
  }

  if (kind == kMskSyntaxKindBadToken) {
    MskDiagnosticBagReportBadCharacter(&lexer->diagnostics, position,
                                       Cur(lexer));
    lexer->position++;
  }

  // The text could have been created already.
  if (StringSize(text) == 0) {
    text = StringFromView(
        StringViewSubstring(lexer->text, position, lexer->position));
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
  if (lexer->position + offset >= StringViewSize(lexer->text)) {
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

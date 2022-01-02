#ifndef MINSK_CODE_ANALYSIS_SYNTAX_KIND_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_KIND_H_

#include <string/string.h>

#define MSK__SYNTAX_KINDS    \
  X(Invalid)                 \
  X(BadToken)                \
  X(EndOfFileToken)          \
  X(NumberToken)             \
  X(WhitespaceToken)         \
  X(PlusToken)               \
  X(MinusToken)              \
  X(StarToken)               \
  X(SlashToken)              \
  X(OpenParenthesisToken)    \
  X(CloseParenthesisToken)   \
                             \
  X(LiteralExpression)       \
  X(BinaryExpression)        \
  X(ParenthesizedExpression) \
  X(UnaryExpression)

/// The concrete syntax kind.
///
/// \see MskSyntaxNodeClass for the hierarchical syntax kind (i.e. Expression
/// vs. Statement).
typedef enum {
#define X(x) kMskSyntaxKind##x,
  MSK__SYNTAX_KINDS
#undef X
} MskSyntaxKind;

StringView MskSyntaxKindName(MskSyntaxKind kind);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_KIND_H_

#ifndef MINSK_CODE_ANALYSIS_SYNTAX_KIND_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_KIND_H_

#include <phyto/collections/dynamic_array.h>
#include <phyto/string/string.h>

#define MSK__SYNTAX_KINDS    \
  /* The zero value. */      \
  X(Invalid)                 \
                             \
  /* Special tokens. */      \
  X(BadToken)                \
  X(EndOfFileToken)          \
                             \
  /* "Dynamic" tokens. */    \
  X(NumberToken)             \
  X(WhitespaceToken)         \
  X(IdentifierToken)         \
                             \
  /* "Static" tokens. */     \
  X(PlusToken)               \
  X(MinusToken)              \
  X(StarToken)               \
  X(SlashToken)              \
  X(OpenParenthesisToken)    \
  X(CloseParenthesisToken)   \
  X(BangToken)               \
  X(AmpersandAmpersandToken) \
  X(PipePipeToken)           \
  X(EqualsEqualsToken)       \
  X(BangEqualsToken)         \
  X(EqualsToken)             \
                             \
  /* Keywords. */            \
  X(TrueKeyword)             \
  X(FalseKeyword)            \
                             \
  /* Syntax tree nodes. */   \
  X(LiteralExpression)       \
  X(BinaryExpression)        \
  X(ParenthesizedExpression) \
  X(UnaryExpression)         \
  X(NameExpression)          \
  X(AssignmentExpression)

/// The concrete syntax kind.
///
/// \see MskSyntaxNodeClass for the hierarchical syntax kind (i.e. Expression
/// vs. Statement).
typedef enum {
#define X(x) kMskSyntaxKind##x,
  MSK__SYNTAX_KINDS
#undef X
} MskSyntaxKind;

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_DECL(MskSyntaxKinds, MskSyntaxKind);

extern const MskSyntaxKinds_callbacks_t kMskSyntaxKindsCallbacks;

MskSyntaxKinds_t MskSyntaxKindsGetAll(void);
phyto_string_span_t MskSyntaxKindName(MskSyntaxKind kind);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_KIND_H_

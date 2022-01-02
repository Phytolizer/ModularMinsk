#ifndef MINSK_CODE_ANALYSIS_SYNTAX_NODE_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_NODE_H_

#include <string/string.h>

#include "minsk/code_analysis/syntax/kind.h"

#define MSK__SYNTAX_NODES X(Expression)

typedef enum {
#define X(x) kMskSyntaxNodeKind##x,
  MSK__SYNTAX_NODES
#undef X
} MskSyntaxNodeKind;

StringView MskSyntaxNodeKindName(MskSyntaxNodeKind kind);

/// This struct is pretty much empty, as it's meant to be inherited from.
typedef struct {
  MskSyntaxNodeKind kind;
} MskSyntaxNode;

MskSyntaxKind MskSyntaxNodeGetKind(MskSyntaxNode* node);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_NODE_H_

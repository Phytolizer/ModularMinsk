#ifndef MINSK_CODE_ANALYSIS_SYNTAX_NODE_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_NODE_H_

#include <stdbool.h>
#include <stdio.h>
#include <string/string.h>

#include "minsk/code_analysis/syntax/kind.h"

#define MSK__SYNTAX_NODE_CLASSES \
  X(Expression)           \
  X(Token)

/// The actual type of the node, in the C "class" hierarchy.
///
/// \see MskSyntaxNodeKind for the flattened list of all node kinds.
typedef enum {
#define X(x) kMskSyntaxNodeClass##x,
  MSK__SYNTAX_NODE_CLASSES
#undef X
} MskSyntaxNodeClass;

StringView MskSyntaxNodeClassName(MskSyntaxNodeClass kind);

/// This struct is pretty much empty, as it's meant to be inherited from.
typedef struct {
  MskSyntaxNodeClass cls;
} MskSyntaxNode;

typedef VEC_TYPE(MskSyntaxNode*) MskSyntaxNodeChildren;

MskSyntaxKind MskSyntaxNodeGetKind(MskSyntaxNode* node);
MskSyntaxNodeChildren MskSyntaxNodeGetChildren(MskSyntaxNode* node);
void MskSyntaxNodePrettyPrint(MskSyntaxNode* node, FILE* fp, bool colors);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_NODE_H_

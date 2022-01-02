#include "minsk/code_analysis/syntax/kind.h"

static const char* const kMskSyntaxKindNames[] = {
#define X(x) [kMskSyntaxKind##x] = #x,
    MSK__SYNTAX_KINDS
#undef X
};

StringView MskSyntaxKindName(MskSyntaxKind kind) {
  return StringViewFromC(kMskSyntaxKindNames[kind]);
}

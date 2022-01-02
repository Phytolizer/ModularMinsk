#include "minsk/code_analysis/syntax/kind.h"

static const char* const kMskSyntaxKindNames[] = {
#define X(x) [kMskSyntaxKind##x] = #x,
    MSK__SYNTAX_KINDS
#undef X
};

StringView MskSyntaxKindName(MskSyntaxKind kind) {
  switch (kind) {
#define X(x)              \
  case kMskSyntaxKind##x: \
    return StringViewFromC(kMskSyntaxKindNames[kMskSyntaxKind##x]);
    MSK__SYNTAX_KINDS
#undef X
    default:
      return StringViewFromC("<unknown>");
  }
}

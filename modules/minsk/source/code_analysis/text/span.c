#include "minsk/code_analysis/text/span.h"

uint64_t MskTextSpanEnd(MskTextSpan span) {
  return span.start + span.length;
}

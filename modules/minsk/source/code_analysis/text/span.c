#include "minsk/code_analysis/text/span.h"

MskTextSpan MskTextSpanNew(uint64_t start, uint64_t end) {
  return (MskTextSpan){
      .start = start,
      .length = end - start,
  };
}

uint64_t MskTextSpanEnd(MskTextSpan span) {
  return span.start + span.length;
}

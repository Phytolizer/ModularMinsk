#ifndef MINSK_CODE_ANALYSIS_TEXT_SPAN_H_
#define MINSK_CODE_ANALYSIS_TEXT_SPAN_H_

#include <stdint.h>

typedef struct {
  uint64_t start;
  uint64_t length;
} MskTextSpan;

uint64_t MskTextSpanEnd(MskTextSpan span);

#endif  // MINSK_CODE_ANALYSIS_TEXT_SPAN_H_

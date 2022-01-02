#include "span/span.h"

size_t SpanSize(SpanUnpacked s) {
  return (s.end - s.begin) / s.sizeof_t;
}

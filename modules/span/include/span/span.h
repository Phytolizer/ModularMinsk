#ifndef SPAN_SPAN_H_
#define SPAN_SPAN_H_

#include <stddef.h>
#include <stdint.h>

typedef struct {
  const uint8_t* begin;
  const uint8_t* end;
  size_t sizeof_t;
} SpanUnpacked;

#define SPAN_TYPE(T) \
  struct {           \
    const T* begin;  \
    const T* end;    \
  }

#define SPAN_UNPACK(S)                     \
  ((SpanUnpacked){                         \
      .begin = (const uint8_t*)(S)->begin, \
      .end = (const uint8_t*)(S)->end,     \
      .sizeof_t = sizeof(*(S)->begin),     \
  })

#define SPAN_SIZE(S) ((S)->end - (S)->begin)

#endif  // SPAN_SPAN_H_

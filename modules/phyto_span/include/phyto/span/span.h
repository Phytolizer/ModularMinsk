#ifndef SPAN_SPAN_H_
#define SPAN_SPAN_H_

#include <stddef.h>
#include <stdint.h>

typedef struct {
  const uint8_t* begin;
  const uint8_t* end;
  size_t size;
  size_t sizeof_t;
} SpanUnpacked;

#define PHYTO_SPAN_TYPE(T) \
  struct {                 \
    const T* begin;        \
    const T* end;          \
    size_t size;           \
  }

#define PHYTO_SPAN_NEW(Begin, End) \
  { .begin = Begin, .end = End, .size = (End) - (Begin) }

#define PHYTO_SPAN_EMPTY \
  { 0 }

#define PHYTO_SPAN_UNPACK(S)              \
  ((SpanUnpacked){                        \
      .begin = (const uint8_t*)(S).begin, \
      .end = (const uint8_t*)(S).end,     \
      .size = (S).end - (S).begin,        \
      .sizeof_t = sizeof(*(S).begin),     \
  })

#define PHYTO_SPAN_SIZE(S) ((S).size)

#endif  // SPAN_SPAN_H_

#include "vec/vec.h"

#include <stdlib.h>
#include <string.h>

bool VecExpand(VecUnpacked v) {
  if (*v.size + 1 > *v.capacity) {
    uint64_t n = (*v.capacity == 0) ? 1 : *v.capacity * 2;
    return VecReserve(v, n);
  }
  return true;
}

bool VecReserve(VecUnpacked v, uint64_t amount) {
  if (*v.capacity < amount) {
    void* ptr = realloc(*v.data, amount * v.sizeof_t);
    if (ptr == NULL) {
      return false;
    }
    *v.data = ptr;
    *v.capacity = amount;
  }
  return true;
}

bool VecAppend(VecUnpacked v, const void* data, uint64_t size) {
  if (!VecReserve(v, *v.size + size)) {
    return false;
  }
  memcpy(*v.data + *v.size * v.sizeof_t, data, size * v.sizeof_t);
  *v.size += size;
  return true;
}

#include "hash/hash.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

enum { kInitialCapacity = 16, kMaxLoad = 0xBF };

static bool HashRehash(HashUnpacked hash, uint64_t new_capacity);
static bool HashKeyVecEqualSpan(HashKeyVec a, HashKeySpan b);
static uint64_t HashKeySpanHash(HashKeySpan key);
static HashKey HashOwnKey(HashKeyView key);

HashAddResult HashAdd(HashUnpacked hash,
                      HashKeyView key,
                      const uint8_t* value) {
  if (*hash.capacity == 0 || *hash.size * 0x100 / *hash.capacity > kMaxLoad) {
    if (!HashRehash(hash,
                    *hash.capacity ? *hash.capacity * 2 : kInitialCapacity)) {
      return kHashAddFailure;
    }
  }

  uint64_t index = key.hash % *hash.capacity;
  while (VEC_SIZE(&(*hash.keys)[index].vec) != 0) {
    if ((*hash.keys)[index].hash == key.hash &&
        HashKeyVecEqualSpan((*hash.keys)[index].vec, key.span)) {
      memcpy(&(*hash.values)[index * hash.sizeof_value], value,
             hash.sizeof_value);
      return kHashAddReplace;
    }
    index = (index + 1) % *hash.capacity;
  }

  (*hash.keys)[index] = HashOwnKey(key);
  memcpy(&(*hash.values)[index * hash.sizeof_value], value, hash.sizeof_value);
  hash.size++;
  return kHashAddSuccess;
}

HashKeyView HashCreateKey(HashKeySpan key) {
  return (HashKeyView){
      .span = key,
      .hash = HashKeySpanHash(key),
  };
}

void HashFree(HashUnpacked hash) {
  for (uint64_t i = 0; i < *hash.capacity; i++) {
    VEC_FREE(&(*hash.keys)[i].vec);
  }
  free(*hash.keys);
  free(*hash.values);
}

bool HashGet(HashUnpacked hash, HashKeyView key, uint8_t* out_value) {
  if (*hash.capacity == 0) {
    return false;
  }
  uint64_t index = key.hash % *hash.capacity;
  while (VEC_SIZE(&(*hash.keys)[index].vec) != 0) {
    if ((*hash.keys)[index].hash == key.hash &&
        HashKeyVecEqualSpan((*hash.keys)[index].vec, key.span)) {
      memcpy(out_value, &(*hash.values)[index * hash.sizeof_value],
             hash.sizeof_value);
      return true;
    }
    index = (index + 1) % *hash.capacity;
  }
  return false;
}

bool HashRehash(HashUnpacked hash, uint64_t new_capacity) {
  HashKey* new_keys = calloc(new_capacity, sizeof(HashKey));
  if (!new_keys) {
    return false;
  }

  uint8_t* new_values = calloc(new_capacity, hash.sizeof_value);
  if (!new_values) {
    free(new_keys);
    return false;
  }

  for (uint64_t i = 0; i < *hash.capacity; i++) {
    if (VEC_SIZE(&(*hash.keys)[i].vec) != 0) {
      uint64_t index = (*hash.keys)[i].hash % new_capacity;
      while (VEC_SIZE(&new_keys[index].vec) != 0) {
        index = (index + 1) % new_capacity;
      }
      new_keys[index] = (*hash.keys)[i];
      memcpy(&new_values[index * hash.sizeof_value],
             &(*hash.values)[i * hash.sizeof_value], hash.sizeof_value);
    }
  }

  free(*hash.keys);
  free(*hash.values);
  *hash.keys = new_keys;
  *hash.values = new_values;
  *hash.capacity = new_capacity;
  return true;
}

bool HashKeyVecEqualSpan(HashKeyVec a, HashKeySpan b) {
  return VEC_SIZE(&a) == SPAN_SIZE(b) &&
         memcmp(a.data, b.begin, VEC_SIZE(&a)) == 0;
}

uint64_t HashKeySpanHash(HashKeySpan key) {
  // FNV-1a 64-bit hash
  // https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
  uint64_t hash = 0xcbf29ce484222325;
  for (uint64_t i = 0; i < SPAN_SIZE(key); i++) {
    hash ^= key.begin[i];
    hash *= 0x100000001b3;
  }
  return hash;
}

HashKey HashOwnKey(HashKeyView key) {
  HashKey hash_key = {.hash = key.hash};
  VEC_APPEND(&hash_key.vec, key.span.begin, SPAN_SIZE(key.span));
  return hash_key;
}

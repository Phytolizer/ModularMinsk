#include "phyto/hash/hash.h"

uint64_t phyto_hash_fnv1a(phyto_string_span_t s) {
    uint64_t hash = 0xcbf29ce484222325;
    for (size_t i = 0; i < s.size; i++) {
        hash ^= (uint8_t)s.begin[i];
        hash *= 0x100000001b3;
    }
    return hash;
}

uint64_t phyto_hash_djb2(phyto_string_span_t s) {
    uint64_t hash = 5381;
    for (size_t i = 0; i < s.size; i++) {
        hash = ((hash << 5) + hash) + (uint8_t)s.begin[i];
    }
    return hash;
}


const double phyto_hash_default_load = 0.75;

static const char* const hash_flag_descriptions[] = {
#define X(x, y) y,
    PHYTO_HASH_FLAGS_X
#undef X
};

phyto_string_span_t phyto_hash_flag_explain(phyto_hash_flag_t flag) {
    return phyto_string_span_from_c(hash_flag_descriptions[flag]);
}

static const uint64_t primes[] = {
    53,
    97,
    191,
    383,
    769,
    1531,
    3067,
    6143,
    12289,
    24571,
    49157,
    98299,
    196613,
    393209,
    786431,
    1572869,
    3145721,
    6291449,
    12582917,
    25165813,
    50331653,
    100663291,
    201326611,
    402653189,
    805306357,
    1610612741,
    3221225473,
    6442450939,
    12884901893,
    25769803799,
    51539607551,
    103079215111,
    206158430209,
    412316860441,
    824633720831,
    1649267441651,
    3298534883309,
    6597069766657,
    13194139533299,
    26388279066623,
    52776558133303,
    105553116266489,
    211106232532969,
    422212465066001,
    844424930131963,
    1688849860263953,
    3377699720527861,
    6755399441055731,
    13510798882111483,
    27021597764222939,
    54043195528445957,
    108086391056891903,
    216172782113783773,
    432345564227567621,
    864691128455135207,
    1729382256910270481,
    3458764513820540933,
    6917529027641081903,
    13835058055282163729LLU,
};

phyto_hash_prime_span_t phyto_hash_prime_span(void) {
    return (phyto_hash_prime_span_t)PHYTO_SPAN_NEW(primes,
                                                   primes + sizeof(primes) / sizeof(*primes));
}

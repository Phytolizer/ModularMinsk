#include "nonstd/qsort.h"

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Reference: glibc qsort <https://code.woboq.org/userspace/glibc/stdlib/qsort.c.html>

#define ISORT_THRESHOLD 32

typedef struct {
    uint8_t* low;
    uint8_t* high;
} stack_node_t;

#define STACK_SIZE (CHAR_BIT * sizeof(size_t))
#define PUSH(Lo, Hi)      \
    do {                  \
        top->low = (Lo);  \
        top->high = (Hi); \
        ++top;            \
    } while (false)
#define POP(Lo, Hi)       \
    do {                  \
        --top;            \
        (Lo) = top->low;  \
        (Hi) = top->high; \
    } while (false)
#define STACK_NOT_EMPTY (top > stack)
#define SWAP(a, b, size)       \
    do {                       \
        char tmp_[size];       \
        memcpy(tmp_, a, size); \
        memcpy(a, b, size);    \
        memcpy(b, tmp_, size);  \
    } while (false)

void nonstd_qsort_r(void* base,
                    size_t nmemb,
                    size_t size,
                    int (*cmp)(const void*, const void*, void*),
                    void* arg) {
    uint8_t* b = (uint8_t*)base;
    const size_t isort_threshold = ISORT_THRESHOLD * size;
    if (nmemb <= 1) {
        return;
    }
    if (nmemb > ISORT_THRESHOLD) {
        uint8_t* low = b;
        uint8_t* high = &b[size * (nmemb - 1)];
        stack_node_t stack[STACK_SIZE];
        stack_node_t* top = stack;

        PUSH(NULL, NULL);

        while (STACK_NOT_EMPTY) {
            uint8_t* mid = low + size * ((high - low) / size >> 1);

            if (cmp(mid, low, arg) < 0) {
                SWAP(mid, low, size);
            }
            if (cmp(high, mid, arg) < 0) {
                SWAP(mid, high, size);
                if (cmp(mid, low, arg) < 0) {
                    SWAP(mid, low, size);
                }
            }

            uint8_t* left = low + size;
            uint8_t* right = high - size;

            do {
                while (cmp(left, mid, arg) < 0) {
                    left += size;
                }
                while (cmp(mid, right, arg) < 0) {
                    right -= size;
                }
                if (left < right) {
                    SWAP(left, right, size);
                    if (mid == left) {
                        mid = right;
                    } else if (mid == right) {
                        mid = left;
                    }
                    left += size;
                    right -= size;
                } else if (left == right) {
                    left += size;
                    right -= size;
                    break;
                }
            } while (left <= right);

            if ((size_t)(right - low) <= isort_threshold) {
                if ((size_t)(high - left) <= isort_threshold) {
                    POP(low, high);
                } else {
                    low = left;
                }
            } else if ((size_t)(high - left) <= isort_threshold) {
                high = right;
            } else if ((right - low) > (high - left)) {
                PUSH(low, right);
                low = left;
            } else {
                PUSH(left, high);
                high = right;
            }
        }
    }

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

    uint8_t* const endptr = &b[size * (nmemb - 1)];
    uint8_t* tmp = b;
    uint8_t* thresh = MIN(endptr, b + isort_threshold);
    uint8_t* runptr;

    for (runptr = tmp + size; runptr <= thresh; runptr += size) {
        if (cmp(runptr, tmp, arg) < 0) {
            tmp = runptr;
        }
    }
    if (tmp != b) {
        SWAP(tmp, b, size);
    }

    runptr = b + size;
    while ((runptr += size) <= endptr) {
        tmp = runptr - size;
        while (cmp(runptr, tmp, arg) < 0) {
            tmp -= size;
        }
        tmp += size;
        if (tmp != runptr) {
            uint8_t* trav = runptr + size;
            while (--trav >= runptr) {
                uint8_t c = *trav;
                uint8_t* hi = trav;
                uint8_t* lo = trav - size;
                while (lo >= tmp) {
                    *hi = *lo;
                    hi = lo;
                    lo -= size;
                }
                *hi = c;
            }
        }
    }
}

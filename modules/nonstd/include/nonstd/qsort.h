#ifndef NONSTD_QSORT_H_
#define NONSTD_QSORT_H_

#include <stddef.h>

void nonstd_qsort_r(void* base,
                    size_t nmemb,
                    size_t size,
                    int (*cmp)(const void*, const void*, void*),
                    void* arg);

#endif  // NONSTD_QSORT_H_

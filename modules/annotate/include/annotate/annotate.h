#ifndef ANNOTATE_ANNOTATE_H_
#define ANNOTATE_ANNOTATE_H_

#ifdef __GNUC__
#define ANNOTATE_NONNULL(x) __attribute__((nonnull(x)))
#else
#define ANNOTATE_NONNULL(x)
#endif

#endif  // ANNOTATE_ANNOTATE_H_

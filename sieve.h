#ifndef SIEVE_H
#define SIEVE_H

#include "types.h"

typedef void (*callback)(void *ctx, u64 prime);

/* Segment size must be within [1, 2^29]. */
u64  sieve_count(u64 lower, u64 upper, u32 segment_size, u32 max_threads);

/* Segment size must be within [1, 2^29]. */
void sieve_generate(u64 lower,
                    u64 upper,
                    u32 segment_size,
                    callback cb,
                    void *ctx);

#endif

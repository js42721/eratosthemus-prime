#ifndef SEGMENT_H
#define SEGMENT_H

#include "types.h"

typedef struct segment
{
    u8 *data;
    u32 size;
    u64 lower;
    u64 lower_div;
    u64 upper;
    u64 upper_div;
} segment;

typedef void (*callback)(void *ctx, u64 prime);

segment *segment_new(u32 max_size);

segment *segment_bootstrap(u32 upper, u8 *magic, u32 magic_size);

void     segment_init(segment *s,
                      u8 *magic,
                      u32 magic_size,
                      u64 lower_div,
                      u64 upper_div);

void     segment_sieve(segment *s, u32 *primes, u32 primes_size);

void     segment_trim_lower(segment *s, u64 lower);

void     segment_trim_upper(segment *s, u64 upper);

void     segment_extract(segment *s, callback cb, void *ctx);

u32      segment_count(segment *s);

void     segment_free(segment *s);

#endif

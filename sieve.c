#include "sieve.h"

#include <math.h>
#include <omp.h>
#include <stdlib.h>

#include "magic.h"
#include "segment.h"
#include "utils.h"

#define MAX_UPPER 18446744073709551600u /* (2^64 - 1) / 30 x 30 */

typedef struct kit
{
    u8  *magic;
    u32  magic_size;
    u32 *primes;
    u32  primes_size;
} kit;

static const u32 pi[30] =
{
    0, 0, 1, 2, 2, 3, 3, 4, 4, 4,
    4, 5, 5, 6, 6, 6, 6, 7, 7, 8,
    8, 8, 8, 9, 9, 9, 9, 9, 9, 10
};

static const u32 small[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

static void enlist_prime(void *ctx, u64 prime)
{
    kit *k;

    k = (kit *)ctx;
    k->primes[k->primes_size++] = prime;
}

static kit *kit_new(u32 upper, u32 segment_size)
{
    kit *k;
    segment *s;
    u32 sqrt_upper;
    u32 limit;
    u32 end;
    u32 i;

    k = ez_malloc(sizeof(*k));
    k->magic = magic_new(&k->magic_size);
    k->primes = ez_malloc(pi_upper(upper) * sizeof(*k->primes));
    k->primes_size = 0;

    sqrt_upper = sqrt(upper);

    /* Non-segmented bootstrap sieve. */
    s = segment_bootstrap(sqrt_upper, k->magic, k->magic_size);
    segment_extract(s, enlist_prime, k);
    segment_free(s);

    s = segment_new(segment_size);
    end = (upper - 1) / 30 + 1;
    limit = (end < segment_size) ? 0 : end - (segment_size - 1);

    /* Starts segmented sieving from where the bootstrap sieve left off. */
    for (i = sqrt_upper / 30 + 1; i < limit; i += segment_size) {
        segment_init(s, k->magic, k->magic_size, i, i + segment_size);
        segment_sieve(s, k->primes, k->primes_size);
        segment_extract(s, enlist_prime, k);
    }

    if (i < end) {
        segment_init(s, k->magic, k->magic_size, i, end);
        segment_sieve(s, k->primes, k->primes_size);
        segment_trim_upper(s, upper);
        segment_extract(s, enlist_prime, k);
    }

    segment_free(s);

    k->primes = ez_realloc(k->primes, k->primes_size * sizeof(*k->primes));

    return k;
}

static void kit_free(kit *k)
{
    magic_free(k->magic);
    free(k->primes);
    free(k);
}

/* lower >= 30, upper <= MAX_UPPER */
static u64 sieve_count_range(kit *k, u64 lower, u64 upper, u32 segment_size)
{
    segment *s;
    u64 result;
    u64 start;
    u64 end;
    u64 limit;
    u64 i;

    result = 0;
    start = lower / 30;
    end = (upper - 1) / 30 + 1;
    limit = (end < segment_size) ? 0 : end - (segment_size - 1);

    /* Segment upper bound must be exact to prevent overflow. */
    s = segment_new(segment_size);
    segment_init(s, k->magic, k->magic_size,
                 start, MIN(start + segment_size, end));
    segment_sieve(s, k->primes, k->primes_size);
    segment_trim_lower(s, lower);

    for (i = start + segment_size; i < limit; i += segment_size) {
        result += segment_count(s);
        segment_init(s, k->magic, k->magic_size, i, i + segment_size);
        segment_sieve(s, k->primes, k->primes_size);
    }

    if (i < end) {
        result += segment_count(s);
        segment_init(s, k->magic, k->magic_size, i, end);
        segment_sieve(s, k->primes, k->primes_size);
    }

    segment_trim_upper(s, upper);
    result += segment_count(s);
    segment_free(s);

    return result;
}

/* lower >= 30, upper <= MAX_UPPER */
static void sieve_generate_range(kit *k,
                                 u64 lower,
                                 u64 upper,
                                 u32 segment_size,
                                 callback cb,
                                 void *ctx)
{
    segment *s;
    u64 start;
    u64 end;
    u64 limit;
    u64 i;

    start = lower / 30;
    end = (upper - 1) / 30 + 1;
    limit = (end < segment_size) ? 0 : end - (segment_size - 1);

    s = segment_new(segment_size);
    segment_init(s, k->magic, k->magic_size,
                 start, MIN(start + segment_size, end));
    segment_sieve(s, k->primes, k->primes_size);
    segment_trim_lower(s, lower);

    for (i = start + segment_size; i < limit; i += segment_size) {
        segment_extract(s, cb, ctx);
        segment_init(s, k->magic, k->magic_size, i, i + segment_size);
        segment_sieve(s, k->primes, k->primes_size);
    }

    if (i < end) {
        segment_extract(s, cb, ctx);
        segment_init(s, k->magic, k->magic_size, i, end);
        segment_sieve(s, k->primes, k->primes_size);
    }

    segment_trim_upper(s, upper);
    segment_extract(s, cb, ctx);
    segment_free(s);
}

u64 sieve_count(u64 lower, u64 upper, u32 segment_size, u32 max_threads)
{
    kit *k;
    u64 result;
    u64 range;
    u64 segments;
    u32 threads;
    u64 interval;
    u64 start;
    u64 end;
    u32 i;

    if (lower > upper || lower > MAX_UPPER)
        return 0;

    lower = MAX(lower, 1);

    if (upper < 30)
        return pi[upper] - pi[lower - 1];

    result = (lower >= 30) ? 0 : 10 - pi[lower - 1];
    lower = MAX(lower, 30);
    upper = MIN(upper, MAX_UPPER);

    k = kit_new(sqrtl(upper), segment_size);

    range = upper - lower;
    segments = ceill((long double)range / (segment_size * 30));
    threads = omp_get_num_procs();
    threads = MIN(threads, segments);
    threads = MIN(threads, max_threads);
    threads = MAX(threads, 1);
    omp_set_num_threads(threads);
    interval = ceill((long double)range / threads);

    #pragma omp parallel for reduction(+: result) private(start, end)
    for (i = 0; i < threads; ++i) {
        start = lower + (interval * i);
        end = (start + interval >= upper) ? upper : start + (interval - 1);
        result += sieve_count_range(k, start, end, segment_size);
    }

    kit_free(k);

    return result;
}

void sieve_generate(u64 lower,
                    u64 upper,
                    u32 segment_size,
                    callback cb,
                    void *ctx)
{
    kit *k;
    u32 i;

    if (lower > upper || lower > MAX_UPPER)
        return;

    if (lower < 30)
        for (i = 0; i < 10; ++i) {
            if (upper < small[i])
                return;
            if (lower <= small[i])
                cb(ctx, small[i]);
        }

    lower = MAX(lower, 30);
    upper = MIN(upper, MAX_UPPER);

    k = kit_new(sqrtl(upper), segment_size);

    sieve_generate_range(k, lower, upper, segment_size, cb, ctx);

    kit_free(k);
}

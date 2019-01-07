#include "sieve.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "magic.h"
#include "prime.h"
#include "utils.h"
#include "wheel.h"

#define IS_PRIME(b, n) ((b) & (1 << (n)))

/* pi(x) for x < 30. */
static const u32 small[30] =
{
    0, 0, 1, 2, 2, 3, 3, 4, 4, 4, 4, 5, 5, 6, 6,
    6, 6, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 10
};

static const u32 z[8] = { 0, 1, 4, 5, 9, 12, 17, 28 };

static u32 numeric_val(u32 div, u32 mod)
{
    return div * 30 + wheel[mod];
}

static u32 find_square(u32 div, u32 mod)
{
    return div * (div * 30 + wheel[mod] * 2) + z[mod];
}

static struct prime *bootstrap(u32 upper, u32 *count)
{
    struct prime *primes;
    struct prime *trimmed;
    u8 *sieve;
    u32 sieve_size;
    u32 sieve_limit;
    u32 i, j, k;

    sieve_size = upper / 30 + 1;
    sieve = malloc(sieve_size);
    if (!sieve)
        return NULL;

    /* The extra space is for excess primes in the last byte. */
    primes = malloc((pi_upper(upper) + 7) * sizeof(*primes));
    if (!primes)
        goto cleanup_sieve;

    k = 0;
    sieve_limit = sqrt(upper) / 30 + 1;
    memset(sieve, 0xff, sieve_size);
    sieve[0] = 0xfe; /* Marks 1 as not prime. */

    for (i = 0; i < sieve_limit; ++i)
        for (j = 0; j < 8; ++j) {
            if (!IS_PRIME(sieve[i], j))
                continue;
            /* Starts at the square. */
            init_prime(&primes[k], i, j, j, find_square(i, j));
            mark_multiples(sieve, sieve_size, &primes[k]);
            ++k;
        }

    for (; i < sieve_size; ++i)
        for (j = 0; j < 8; ++j) {
            if (!IS_PRIME(sieve[i], j))
                continue;
            init_prime(&primes[k], i, j, j, find_square(i, j) - sieve_size);
            ++k;
        }

    *count = k;

    /* It's okay if the trim fails. */
    trimmed = realloc(primes, k * sizeof(*primes));
    if (trimmed)
        primes = trimmed;

cleanup_sieve:
    free(sieve);

    return primes;
}

s32 sieve(u32 upper, u32 sieve_size)
{
    struct prime *primes;
    u8 *mask;
    u8 *sieve;
    s32 result;
    u32 sqrt_upper;
    u32 primes_size;
    u32 mask_size;
    u32 limit;
    u32 end;
    u32 i, j;

    if (upper < 30)
        return small[upper];

    result = -1;

    mask = magic_mask(&mask_size);
    if (!mask)
        goto out;

    sqrt_upper = sqrt(upper);
    primes = bootstrap(sqrt_upper, &primes_size);
    if (!primes)
        goto cleanup_mask;

    sieve = malloc(sieve_size);
    if (!sieve)
        goto cleanup_primes;

    result = primes_size + 3;
    limit = upper / 30 + 1;

    for (i = sqrt_upper / 30 + 1; i < limit; i += sieve_size) {
        apply_magic_mask(sieve, sieve_size, mask, mask_size, i);
        for (j = MAGIC_MASK_PRIMES; j < primes_size; ++j)
            mark_multiples(sieve, sieve_size, &primes[j]);
        result += popcount(sieve, sieve_size);
    }

    end = limit - (i - sieve_size);

    /* Excludes excess primes found in the last byte. */
    for (i = 8; i--;) {
        if (!IS_PRIME(sieve[end - 1], i))
            continue;
        /* None of the numbers that would cause overflow here are prime. */
        if (numeric_val(limit - 1, i) <= upper)
            break;
        --result;
    }

    /* Excludes excess primes found in the rest of the last segment. */
    result -= popcount(&sieve[end], sieve_size - end);

    free(sieve);
cleanup_primes:
    free(primes);
cleanup_mask:
    free(mask);
out:
    return result;
}

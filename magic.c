#include "magic.h"

#include <stdlib.h>
#include <string.h>

#include "wheel.h"

u8 *magic_mask(u32 *mask_size)
{
    struct prime p;
    u8 *mask;
    u32 lcm;
    u32 i;

    lcm = 1;
    for (i = 1; i <= MAGIC_MASK_PRIMES; ++i)
        lcm *= wheel[i];

    mask = malloc(lcm);
    if (!mask)
        return NULL;

    *mask_size = lcm;
    memset(mask, 0xff, lcm);

    for (i = 1; i <= MAGIC_MASK_PRIMES; ++i) {
        init_prime(&p, 0, i, 0, 0);
        mark_multiples(mask, lcm, &p);
    }

    return mask;
}

void apply_magic_mask(u8 *sieve,
                      u32 sieve_size,
                      u8 *mask,
                      u32 mask_size,
                      u32 index)
{
    u32 sieve_remainder;
    u32 mask_remainder;
    u32 limit;
    u32 mi;

    mi = index % mask_size;
    mask_remainder = mask_size - mi;
    if (mask_remainder >= sieve_size) {
        memcpy(sieve, &mask[mi], sieve_size);
        return;
    }

    memcpy(sieve, &mask[mi], mask_remainder);

    sieve_remainder = sieve_size - mask_remainder;
    if (sieve_remainder <= mask_size) {
        memcpy(&sieve[mask_remainder], mask, sieve_remainder);
        return;
    }

    mi = mask_remainder;
    limit = sieve_size - mask_size;
    do {
        memcpy(&sieve[mi], mask, mask_size);
        mi += mask_size;
    } while (mi < limit);

    memcpy(&sieve[mi], mask, sieve_size - mi);
}

#ifndef WHEEL_H
#define WHEEL_H

#include "types.h"

typedef struct prime
{
    u32 div    : 29;
    u32 mod    :  3;
    u32 step   :  3;
    u32 offset : 29;
} prime;

extern const u32 wheel[8];

void init_prime(prime *p, u32 div, u32 mod, u32 step, u32 offset);

void mark_multiples(prime *p, u8 *sieve, u32 sieve_size);

#endif

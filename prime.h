#ifndef PRIME_H
#define PRIME_H

#include "types.h"

struct prime
{
    u32 div    : 29;
    u32 mod    :  3;
    u32 step   :  3;
    u32 offset : 29;
};

void init_prime(struct prime *p, u32 div, u32 mod, u32 step, u32 offset);

#endif

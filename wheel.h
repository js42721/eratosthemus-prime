#ifndef WHEEL_H
#define WHEEL_H

#include "prime.h"
#include "types.h"

extern const int wheel[8];

void mark_multiples(u8 *sieve, u32 sieve_size, struct prime *prime);

#endif

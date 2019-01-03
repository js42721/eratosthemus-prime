#ifndef MAGIC_H
#define MAGIC_H

#include "types.h"

#define MAGIC_MASK_PRIMES 5 /* [1, 7] */

u8 *magic_mask(u32 *mask_size);

void apply_magic_mask(u8 *sieve,
                      u32 sieve_size,
                      u8 *mask,
                      u32 mask_size,
                      u32 index);

#endif

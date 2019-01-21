#ifndef MAGIC_H
#define MAGIC_H

#include "types.h"

#define MAGIC_PRIMES 5 /* [1, 7] */

u8  *magic_new(u32 *size);

void magic_apply(u8 *m, u32 m_size, u8 *dst, u32 dst_size, u64 i);

void magic_free(u8 *m);

#endif

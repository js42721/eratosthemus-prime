#ifndef UTILS_H
#define UTILS_H

#include "types.h"

int is_prime(u8 val, int pos);

u32 numeric_val(u32 index, u32 bit);

u32 square_index(u32 index, u32 bit);

u32 pi_upper(u32 n);

u32 popcount(u8 *a, u32 start, u32 end);

#endif

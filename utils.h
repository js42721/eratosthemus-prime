#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

#include "types.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

u32   popcount(u8 *a, u32 n);

u32   pi_upper(u32 n);

void *ez_malloc(size_t n);

void *ez_realloc(void *p, size_t n);

#endif

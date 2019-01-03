#include "prime.h"

void init_prime(struct prime *p, u32 index, u32 bit, u32 step, u32 offset)
{
    p->index  = index;
    p->bit    = bit;
    p->step   = step;
    p->offset = offset;
}

#include "prime.h"

void init_prime(struct prime *p, u32 div, u32 mod, u32 step, u32 offset)
{
    p->div = div;
    p->mod = mod;
    p->step = step;
    p->offset = offset;
}

#include "magic.h"

#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "wheel.h"

u8 *magic_new(u32 *size)
{
    prime p;
    u8 *m;
    u32 lcm;
    u32 i;

    lcm = 1;
    for (i = 1; i <= MAGIC_PRIMES; ++i)
        lcm *= wheel[i];

    m = ez_malloc(lcm);

    *size = lcm;
    memset(m, 0xff, lcm);

    for (i = 1; i <= MAGIC_PRIMES; ++i) {
        init_prime(&p, 0, i, 0, 0);
        mark_multiples(&p, m, lcm);
    }

    return m;
}

void magic_apply(u8 *m, u32 m_size, u8 *dst, u32 dst_size, u64 i)
{
    u32 m_rem;
    u32 dst_rem;
    u32 mi;
    u32 limit;

    mi = i % m_size;
    m_rem = m_size - mi;
    if (m_rem >= dst_size) {
        memcpy(dst, &m[mi], dst_size);
        return;
    }

    memcpy(dst, &m[mi], m_rem);

    dst_rem = dst_size - m_rem;
    if (dst_rem <= m_size) {
        memcpy(&dst[m_rem], m, dst_rem);
        return;
    }

    mi = m_rem;
    limit = dst_size - m_size;
    do {
        memcpy(&dst[mi], m, m_size);
        mi += m_size;
    } while (mi < limit);

    memcpy(&dst[mi], m, dst_size - mi);
}

void magic_free(u8 *m)
{
    free(m);
}

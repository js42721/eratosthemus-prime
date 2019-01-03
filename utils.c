#include "utils.h"

#include <math.h>

#include "wheel.h"

/* http://graphics.stanford.edu/~seander/bithacks.html */
static const int b[256] =
{
#   define B2(n) n,     n + 1,     n + 1,     n + 2
#   define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#   define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)
    B6(0), B6(1), B6(1), B6(2)
};

static const int z[8] = { 0, 1, 4, 5, 9, 12, 17, 28 };

int is_prime(u8 val, int pos)
{
    return val & (1 << pos);
}

u32 numeric_val(u32 index, u32 bit)
{
    return index * 30 + wheel[bit];
}

u32 square_index(u32 index, u32 bit)
{
    return index * (index * 30 + wheel[bit] * 2) + z[bit];
}

u32 pi_upper(u32 n)
{
    double log_n;
    /*
     * See "The kth prime is greater than k(ln k + ln ln k - 1) for k >= 2" by
     * Pierre Dusart.
     */
    log_n = log(n);
    return (n / log_n) * (1.2762 / log_n + 1);
}

u32 popcount(u8 *a, u32 start, u32 end)
{
    u32 result;
    u32 i;

    result = 0;
    for (i = start; i < end; ++i)
        result += b[a[i]];

    return result;
}

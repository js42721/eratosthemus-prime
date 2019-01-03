#include "utils.h"

#include <math.h>

static const u32 b[256] =
{/* http://graphics.stanford.edu/~seander/bithacks.html */
#   define B2(n) n,     n + 1,     n + 1,     n + 2
#   define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#   define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)
    B6(0), B6(1), B6(1), B6(2)
};

u32 popcount(u8 *a, u32 start, u32 end)
{
    u32 result;
    u32 i;

    result = 0;
    for (i = start; i < end; ++i)
        result += b[a[i]];

    return result;
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

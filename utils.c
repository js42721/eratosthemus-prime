#include "utils.h"

#include <math.h>
#include <stdio.h>

static const u8 popcount8[256] =
{/* http://graphics.stanford.edu/~seander/bithacks.html */
#   define B2(n) n,     n + 1,     n + 1,     n + 2
#   define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#   define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)
    B6(0), B6(1), B6(1), B6(2)
};

u32 popcount(u8 *a, u32 n)
{
    u32 result;
    u32 i;

    result = 0;
    for (i = 0; i < n; ++i)
        result += popcount8[a[i]];

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

void *ez_malloc(size_t n)
{
    void *result;

    if (n == 0)
        n = 1;

    result = malloc(n);
    if (!result) {
        perror("Bad malloc");
        exit(EXIT_FAILURE);
    }

    return result;
}

void *ez_realloc(void *p, size_t n)
{
    void *result;

    if (n == 0)
        n = 1;

    result = (!p) ? malloc(n) : realloc(p, n);
    if (!result) {
        perror("Bad realloc");
        exit(EXIT_FAILURE);
    }

    return result;
}

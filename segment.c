#include "segment.h"

#include <stdlib.h>
#include <string.h>

#include "magic.h"
#include "utils.h"
#include "wheel.h"

static const u32 step_finder[30] =
{
    0, 0, 1, 1, 1, 1, 1, 1, 2, 2,
    2, 2, 3, 3, 4, 4, 4, 4, 5, 5,
    6, 6, 6, 6, 7, 7, 7, 7, 7, 7
};

static const u32 square_finder[8] =
{
    0, /*0,   0,   0,   0,   0,   0,   0,*/
  /*0,*/ 1, /*2,   3,   3,   4,   5,   6,*/
  /*0,   2,*/ 4, /*4,   6,   6,   8,  10,*/
  /*0,   3,   4,*/ 5, /*7,   8,   9,  12,*/
  /*0,   3,   6,   7,*/ 9,/*10,  13,  16,*/
  /*0,   4,   6,   8,  10,*/12,/*14,  18,*/
  /*0,   5,   8,   9,  13,  14,*/17,/*22,*/
  /*0,   6,  10,  12,  16,  18,  22,*/28
};

static const s8 set_bits[256][8] =
{
    {-1},              {0,-1},            {1,-1},            {0,1,-1},
    {2,-1},            {0,2,-1},          {1,2,-1},          {0,1,2,-1},
    {3,-1},            {0,3,-1},          {1,3,-1},          {0,1,3,-1},
    {2,3,-1},          {0,2,3,-1},        {1,2,3,-1},        {0,1,2,3,-1},
    {4,-1},            {0,4,-1},          {1,4,-1},          {0,1,4,-1},
    {2,4,-1},          {0,2,4,-1},        {1,2,4,-1},        {0,1,2,4,-1},
    {3,4,-1},          {0,3,4,-1},        {1,3,4,-1},        {0,1,3,4,-1},
    {2,3,4,-1},        {0,2,3,4,-1},      {1,2,3,4,-1},      {0,1,2,3,4,-1},
    {5,-1},            {0,5,-1},          {1,5,-1},          {0,1,5,-1},
    {2,5,-1},          {0,2,5,-1},        {1,2,5,-1},        {0,1,2,5,-1},
    {3,5,-1},          {0,3,5,-1},        {1,3,5,-1},        {0,1,3,5,-1},
    {2,3,5,-1},        {0,2,3,5,-1},      {1,2,3,5,-1},      {0,1,2,3,5,-1},
    {4,5,-1},          {0,4,5,-1},        {1,4,5,-1},        {0,1,4,5,-1},
    {2,4,5,-1},        {0,2,4,5,-1},      {1,2,4,5,-1},      {0,1,2,4,5,-1},
    {3,4,5,-1},        {0,3,4,5,-1},      {1,3,4,5,-1},      {0,1,3,4,5,-1},
    {2,3,4,5,-1},      {0,2,3,4,5,-1},    {1,2,3,4,5,-1},    {0,1,2,3,4,5,-1},
    {6,-1},            {0,6,-1},          {1,6,-1},          {0,1,6,-1},
    {2,6,-1},          {0,2,6,-1},        {1,2,6,-1},        {0,1,2,6,-1},
    {3,6,-1},          {0,3,6,-1},        {1,3,6,-1},        {0,1,3,6,-1},
    {2,3,6,-1},        {0,2,3,6,-1},      {1,2,3,6,-1},      {0,1,2,3,6,-1},
    {4,6,-1},          {0,4,6,-1},        {1,4,6,-1},        {0,1,4,6,-1},
    {2,4,6,-1},        {0,2,4,6,-1},      {1,2,4,6,-1},      {0,1,2,4,6,-1},
    {3,4,6,-1},        {0,3,4,6,-1},      {1,3,4,6,-1},      {0,1,3,4,6,-1},
    {2,3,4,6,-1},      {0,2,3,4,6,-1},    {1,2,3,4,6,-1},    {0,1,2,3,4,6,-1},
    {5,6,-1},          {0,5,6,-1},        {1,5,6,-1},        {0,1,5,6,-1},
    {2,5,6,-1},        {0,2,5,6,-1},      {1,2,5,6,-1},      {0,1,2,5,6,-1},
    {3,5,6,-1},        {0,3,5,6,-1},      {1,3,5,6,-1},      {0,1,3,5,6,-1},
    {2,3,5,6,-1},      {0,2,3,5,6,-1},    {1,2,3,5,6,-1},    {0,1,2,3,5,6,-1},
    {4,5,6,-1},        {0,4,5,6,-1},      {1,4,5,6,-1},      {0,1,4,5,6,-1},
    {2,4,5,6,-1},      {0,2,4,5,6,-1},    {1,2,4,5,6,-1},    {0,1,2,4,5,6,-1},
    {3,4,5,6,-1},      {0,3,4,5,6,-1},    {1,3,4,5,6,-1},    {0,1,3,4,5,6,-1},
    {2,3,4,5,6,-1},    {0,2,3,4,5,6,-1},  {1,2,3,4,5,6,-1},  {0,1,2,3,4,5,6,-1},
    {7,-1},            {0,7,-1},          {1,7,-1},          {0,1,7,-1},
    {2,7,-1},          {0,2,7,-1},        {1,2,7,-1},        {0,1,2,7,-1},
    {3,7,-1},          {0,3,7,-1},        {1,3,7,-1},        {0,1,3,7,-1},
    {2,3,7,-1},        {0,2,3,7,-1},      {1,2,3,7,-1},      {0,1,2,3,7,-1},
    {4,7,-1},          {0,4,7,-1},        {1,4,7,-1},        {0,1,4,7,-1},
    {2,4,7,-1},        {0,2,4,7,-1},      {1,2,4,7,-1},      {0,1,2,4,7,-1},
    {3,4,7,-1},        {0,3,4,7,-1},      {1,3,4,7,-1},      {0,1,3,4,7,-1},
    {2,3,4,7,-1},      {0,2,3,4,7,-1},    {1,2,3,4,7,-1},    {0,1,2,3,4,7,-1},
    {5,7,-1},          {0,5,7,-1},        {1,5,7,-1},        {0,1,5,7,-1},
    {2,5,7,-1},        {0,2,5,7,-1},      {1,2,5,7,-1},      {0,1,2,5,7,-1},
    {3,5,7,-1},        {0,3,5,7,-1},      {1,3,5,7,-1},      {0,1,3,5,7,-1},
    {2,3,5,7,-1},      {0,2,3,5,7,-1},    {1,2,3,5,7,-1},    {0,1,2,3,5,7,-1},
    {4,5,7,-1},        {0,4,5,7,-1},      {1,4,5,7,-1},      {0,1,4,5,7,-1},
    {2,4,5,7,-1},      {0,2,4,5,7,-1},    {1,2,4,5,7,-1},    {0,1,2,4,5,7,-1},
    {3,4,5,7,-1},      {0,3,4,5,7,-1},    {1,3,4,5,7,-1},    {0,1,3,4,5,7,-1},
    {2,3,4,5,7,-1},    {0,2,3,4,5,7,-1},  {1,2,3,4,5,7,-1},  {0,1,2,3,4,5,7,-1},
    {6,7,-1},          {0,6,7,-1},        {1,6,7,-1},        {0,1,6,7,-1},
    {2,6,7,-1},        {0,2,6,7,-1},      {1,2,6,7,-1},      {0,1,2,6,7,-1},
    {3,6,7,-1},        {0,3,6,7,-1},      {1,3,6,7,-1},      {0,1,3,6,7,-1},
    {2,3,6,7,-1},      {0,2,3,6,7,-1},    {1,2,3,6,7,-1},    {0,1,2,3,6,7,-1},
    {4,6,7,-1},        {0,4,6,7,-1},      {1,4,6,7,-1},      {0,1,4,6,7,-1},
    {2,4,6,7,-1},      {0,2,4,6,7,-1},    {1,2,4,6,7,-1},    {0,1,2,4,6,7,-1},
    {3,4,6,7,-1},      {0,3,4,6,7,-1},    {1,3,4,6,7,-1},    {0,1,3,4,6,7,-1},
    {2,3,4,6,7,-1},    {0,2,3,4,6,7,-1},  {1,2,3,4,6,7,-1},  {0,1,2,3,4,6,7,-1},
    {5,6,7,-1},        {0,5,6,7,-1},      {1,5,6,7,-1},      {0,1,5,6,7,-1},
    {2,5,6,7,-1},      {0,2,5,6,7,-1},    {1,2,5,6,7,-1},    {0,1,2,5,6,7,-1},
    {3,5,6,7,-1},      {0,3,5,6,7,-1},    {1,3,5,6,7,-1},    {0,1,3,5,6,7,-1},
    {2,3,5,6,7,-1},    {0,2,3,5,6,7,-1},  {1,2,3,5,6,7,-1},  {0,1,2,3,5,6,7,-1},
    {4,5,6,7,-1},      {0,4,5,6,7,-1},    {1,4,5,6,7,-1},    {0,1,4,5,6,7,-1},
    {2,4,5,6,7,-1},    {0,2,4,5,6,7,-1},  {1,2,4,5,6,7,-1},  {0,1,2,4,5,6,7,-1},
    {3,4,5,6,7,-1},    {0,3,4,5,6,7,-1},  {1,3,4,5,6,7,-1},  {0,1,3,4,5,6,7,-1},
    {2,3,4,5,6,7,-1},  {0,2,3,4,5,6,7,-1},{1,2,3,4,5,6,7,-1},{/*0,1,...,7,*/-1}
};

static u32 find_square(u32 div, u32 mod)
{
    return div * (div * 30 + wheel[mod] * 2) + square_finder[mod];
}

segment *segment_new(u32 max_size)
{
    segment *s;

    s = ez_malloc(sizeof(*s));
    s->data = ez_malloc(max_size);

    return s;
}

segment *segment_bootstrap(u32 upper, u8 *magic, u32 magic_size)
{
    prime p;
    segment *s;
    u32 size;
    u32 i, j;
    u32 val;
    s32 pos;
    u32 square;

    size = (upper - 1) / 30 + 1;
    s = segment_new(size);
    segment_init(s, magic, magic_size, 0, size);

    s->data[0] &= ~1; /* Marks 1 as not prime. */

    /*
     * If a mask is used, the mask primes will be marked as not prime and will
     * not be extracted. This is good since it saves us the trouble of having
     * to skip those primes later.
     */
    for (i = 0; i < size; ++i) {
        val = s->data[i];
        if (val == 0)
            continue;

        pos = set_bits[val][0];
        j = 1;
        do {
            square = find_square(i, pos);
            if (square >= size)
                return s;

            init_prime(&p, i, pos, pos, square);
            mark_multiples(&p, s->data, size);
            pos = set_bits[val][j++];
        } while (pos != -1);
    }

    return s;
}

void segment_init(segment *s,
                  u8 *magic,
                  u32 magic_size,
                  u64 lower_div,
                  u64 upper_div)
{
    s->size = upper_div - lower_div;
    s->lower = lower_div * 30;
    s->lower_div = lower_div;
    s->upper = upper_div * 30;
    s->upper_div = upper_div;

    if (magic)
        magic_apply(magic, magic_size, s->data, s->size, lower_div);
    else
        memset(s->data, 0xff, s->size);
}

void segment_sieve(segment *s, u32 *primes, u32 primes_size)
{
    prime p;
    u32 n;
    u64 intervals;
    u32 remainder;
    u64 next;
    u32 i;

    for (i = 0; i < primes_size; ++i) {
        n = primes[i];
        /* This assumes the primes are listed in ascending order. */
        if ((u64)n * n >= s->upper)
            return;

        /* Computes the next wheel-aligned multiple >= n^2. */
        intervals = (s->lower - 1) / n + 1;
        intervals = MAX(intervals, n);
        remainder = intervals % 30;
        p.step = step_finder[remainder];
        intervals += wheel[p.step] - remainder;
        next = intervals * n;

        /* The second condition handles overflow. */
        if (next >= s->upper || next < s->lower)
            continue;

        p.div = n / 30;
        p.mod = step_finder[n % 30];
        p.offset = (next - s->lower) / 30;

        mark_multiples(&p, s->data, s->size);
    }
}

void segment_trim_lower(segment *s, u64 lower)
{
    u32 start;
    u32 i;

    if (s->size == 0)
        return;

    start = step_finder[lower % 30];

    for (i = 0; i < start; ++i)
        s->data[0] &= ~(1 << i);
}

void segment_trim_upper(segment *s, u64 upper)
{
    u32 end;
    u32 i;

    if (s->size == 0)
        return;

    end = step_finder[(upper + 1) % 30];
    if (end == 0)
        return;

    for (i = end; i < 8; ++i)
        s->data[s->size - 1] &= ~(1 << i);
}

void segment_extract(segment *s, callback cb, void *ctx)
{
    u32 i, j;
    u32 val;
    s32 pos;

    /*
     * This should not be used on a segment with 0xff bytes (not an issue if
     * the segment was sieved).
     */
    for (i = 0; i < s->size; ++i) {
        val = s->data[i];
        if (val == 0)
            continue;

        pos = set_bits[val][0];
        j = 1;
        do {
            cb(ctx, (s->lower_div + i) * 30 + wheel[pos]);
            pos = set_bits[val][j++];
        } while (pos != -1);
    }
}

u32 segment_count(segment *s)
{
    return popcount(s->data, s->size);
}

void segment_free(segment *s)
{
    free(s->data);
    free(s);
}

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAGIC_MASK_PRIMES 5

#define SIEVE_SIZE 32768

struct prime
{
    uint32_t col    : 29;
    uint32_t row    :  3;
    uint32_t step   :  3;
    uint32_t offset : 29;
};

const uint8_t m[64] =
{
    0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f,
    0xfd, 0xdf, 0xef, 0xfe, 0x7f, 0xf7, 0xfb, 0xbf,
    0xfb, 0xef, 0xfe, 0xbf, 0xfd, 0x7f, 0xf7, 0xdf,
    0xf7, 0xfe, 0xbf, 0xdf, 0xfb, 0xfd, 0x7f, 0xef,
    0xef, 0x7f, 0xfd, 0xfb, 0xdf, 0xbf, 0xfe, 0xf7,
    0xdf, 0xf7, 0x7f, 0xfd, 0xbf, 0xfe, 0xef, 0xfb,
    0xbf, 0xfb, 0xf7, 0x7f, 0xfe, 0xef, 0xdf, 0xfd,
    0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe
};

const uint32_t w[8] = { 1, 7, 11, 13, 17, 19, 23, 29 };

const uint32_t t[8] = { 6, 4, 2, 4, 2, 4, 6, 2 };

const uint32_t z[8] = { 0, 1, 4, 5, 9, 12, 17, 28 };

const uint32_t d[64] =
{
    0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 0, 1, 1, 1, 1,
    2, 2, 0, 2, 0, 2, 2, 1,
    3, 1, 1, 2, 1, 1, 3, 1,
    3, 3, 1, 2, 1, 3, 3, 1,
    4, 2, 2, 2, 2, 2, 4, 1,
    5, 3, 1, 4, 1, 3, 5, 1,
    6, 4, 2, 4, 2, 4, 6, 1
};

/* pi(x) for x <= 29. */
const uint32_t small[30] =
{
    0, 0, 1, 2, 2, 3, 3, 4, 4, 4,
    4, 5, 5, 6, 6, 6, 6, 7, 7, 8,
    8, 8, 8, 9, 9, 9, 9, 9, 9, 10
};

/* Bit count table from http://graphics.stanford.edu/~seander/bithacks.html. */
const uint32_t b[256] =
{
#   define B2(n) n,     n + 1,     n + 1,     n + 2
#   define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#   define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)
    B6(0), B6(1), B6(1), B6(2)
};

uint32_t count_bits(uint8_t* a, uint32_t start, uint32_t end)
{
    uint32_t result;
    uint32_t i;

    result = 0;
    for (i = start; i < end; ++i) {
        result += b[a[i]];
    }

    return result;
}

void init_prime(struct prime* p,
                uint32_t col, uint32_t row,
                uint32_t step, uint32_t offset)
{
    p->col    = col;
    p->row    = row;
    p->step   = step;
    p->offset = offset;
}

struct prime* bootstrap(uint32_t upper, uint32_t* cnt)
{
    struct prime* primes;
    struct prime* trimmed;
    uint8_t* sieve;
    uint32_t jump[8];
    uint32_t pi_upper;
    uint32_t primes_idx;
    uint32_t sieve_size;
    uint32_t sieve_end;
    uint32_t t_offset;
    uint32_t current;
    uint32_t next;
    uint32_t i, j, k;
    double log_upper;

    sieve_size = upper / 30 + 1;
    sieve = malloc(sieve_size);
    if (!sieve) {
        return NULL;
    }

    /* Uses Pierre Dusart's upper bound for pi(x). */
    log_upper = log(upper);
    pi_upper = (upper / log_upper) * (1.2762 / log_upper + 1);

    /* The extra space is for excess primes in the last byte. */
    primes = malloc((pi_upper + 7) * sizeof(*primes));
    if (!primes) {
        goto cleanup_sieve;
    }

    primes_idx = 0;
    sieve_end = sqrt(upper) / 30;
    memset(sieve, 0xff, sieve_size);
    sieve[0] = 0xfe; /* Marks 1 as not prime. */

    for (i = 0; i <= sieve_end; ++i) {
        for (j = 0; j < 8; ++j) { /* Processes each bit. */
            if (!(sieve[i] & (1 << j))) { /* Not prime. */
                continue;
            }

            t_offset = 8 * j;
            
            /* Computes the increments. */
            for (k = 0; k < 8; ++k) {
                jump[k] = i * t[k] + d[t_offset + k];
            }

            /* Starts at the square. */
            current = i * (30 * i + 2 * w[j]) + z[j];
            k = j;
            while (current < sieve_size) {
                sieve[current] &= m[t_offset + k];
                current += jump[k];
                k = (k + 1) % 8;
            }

            init_prime(&primes[primes_idx++], i, j, k, current - sieve_size);
        }
    }

    for (i = sieve_end + 1; i < sieve_size; ++i) {
        for (j = 0; j < 8; ++j) {
            if (!(sieve[i] & (1 << j))) {
                continue;
            }
            next = i * (30 * i + 2 * w[j]) + z[j];
            init_prime(&primes[primes_idx++], i, j, j, next - sieve_size);
        }
    }

    *cnt = primes_idx;

    /* It's okay if the trim fails. */
    trimmed = realloc(primes, primes_idx * sizeof(*primes));
    if (trimmed) {
        primes = trimmed;
    }

cleanup_sieve:
    free(sieve);

    return primes;
}

uint8_t* magic_mask(uint32_t* mask_size)
{
    uint8_t* mask;
    uint32_t lcm;
    uint32_t t_offset;
    uint32_t current;
    uint32_t i, j;

    lcm = 1;
    for (i = 1; i <= MAGIC_MASK_PRIMES; ++i) {
        lcm *= w[i];
    }

    mask = malloc(lcm);
    if (!mask) {
        return NULL;
    }

    *mask_size = lcm;
    memset(mask, 0xff, lcm);

    for (i = 1; i <= MAGIC_MASK_PRIMES; ++i) {
        t_offset = 8 * i;
        current = 0;
        j = 0;
        do {
            mask[current] &= m[t_offset + j];
            current += d[t_offset + j];
            j = (j + 1) % 8;
        } while (current < lcm);
    }

    return mask;
}

void apply_magic_mask(uint8_t* sieve, uint32_t sieve_size,
                      uint8_t* mask, uint32_t mask_size, uint32_t idx)
{
    uint32_t mask_remainder;
    uint32_t sieve_remainder;
    uint32_t mi;
    uint32_t i;

    /* 
     * Note that if the mask is applied to the byte containing the mask primes,
     * the mask primes will be marked as composite.
     */
    mi = idx % mask_size;
    for (i = 0; i < sieve_size; i += mask_remainder) {
        mask_remainder = mask_size - mi;
        sieve_remainder = sieve_size - i;
        if (mask_remainder < sieve_remainder) {
            memcpy(&sieve[i], &mask[mi], mask_remainder);
            mi = 0;
        } else {
            memcpy(&sieve[i], &mask[mi], sieve_remainder);
            mi += sieve_remainder;
        }
    }
}

void sieve_segment(uint8_t* sieve, uint32_t sieve_size,
                   struct prime* primes, uint32_t primes_size)
{   
    uint32_t jump[8];
    uint32_t current;
    uint32_t col;
    uint32_t t_offset;
    uint32_t i, j;

    for (i = MAGIC_MASK_PRIMES; i < primes_size; ++i) {
        current = primes[i].offset;
        if (current >= sieve_size) { /* No multiples in the segment. */
            primes[i].offset -= sieve_size;
            continue;
        }

        col = primes[i].col;
        t_offset = 8 * primes[i].row;
        
        for (j = 0; j < 8; ++j) {
            jump[j] = col * t[j] + d[t_offset + j];
        }

        j = primes[i].step;
        do {
            sieve[current] &= m[t_offset + j];
            current += jump[j];
            j = (j + 1) % 8;
        } while (current < sieve_size);

        primes[i].step = j;
        primes[i].offset = current - sieve_size;
    }
}

int32_t sieve(uint32_t upper, uint32_t sieve_size)
{
    struct prime* primes;
    uint8_t* mask;
    uint8_t* sieve;
    int32_t result;
    uint32_t sqrt_upper;
    uint32_t primes_size;
    uint32_t bootstrap_end;
    uint32_t mask_size;
    uint32_t start;
    uint32_t limit;
    uint32_t end;
    uint32_t i;

    if (upper <= 29) {
        return small[upper];
    }

    result = -1;

    mask = magic_mask(&mask_size);
    if (!mask) {
        goto out;
    }

    sqrt_upper = sqrt(upper);
    primes = bootstrap(sqrt_upper, &primes_size);
    if (!primes) {
        goto cleanup_mask;
    }

    sieve = malloc(sieve_size);
    if (!sieve) {
        goto cleanup_primes;
    }
    
    result = primes_size + 3;
    bootstrap_end = sqrt_upper / 30 + 1;
    limit = upper / 30 + 1;

    /* 
     * Starts sieving from where the bootstrapping sieve left off.
     * When adjusting the starting point, remember to also adjust the
     * mask index and the offsets for the primes.
     */
    for (start = bootstrap_end; start < limit; start += sieve_size) {
        apply_magic_mask(sieve, sieve_size, mask, mask_size, start);
        sieve_segment(sieve, sieve_size, primes, primes_size);
        result += count_bits(sieve, 0, sieve_size);
    }

    /* No underflow since the above loop runs at least once. */
    end = limit - (start - sieve_size);

    /* Excludes excess primes found in the last byte. */
    for (i = 8; i--;) {
        if (!(sieve[end - 1] & (1 << i))) {
            continue;
        }
        /* The few overflowable values aren't prime so no cast is needed. */
        if (30 * (limit - 1) + w[i] <= upper) {
            break;
        }
        --result;
    }

    /* Excludes excess primes found in the rest of the last segment. */
    result -= count_bits(sieve, end, sieve_size);

    free(sieve);
cleanup_primes:
    free(primes);
cleanup_mask:
    free(mask);
out:
    return result;
}

int main(int argc, char** argv)
{
    int32_t ans;
    uint32_t upper;
    clock_t start, end;
    double elapsed;

    if (argc < 2) {
        printf("Usage: %s <upper bound>\n", argv[0]);
        return 0;
    }

    if (!sscanf(argv[1], "%"SCNu32, &upper)) {
        fprintf(stderr, "Invalid argument\n");
        return EXIT_FAILURE;
    }
    
    start = clock();

    ans = sieve(upper, SIEVE_SIZE);

    end = clock();

    if (ans == -1) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    printf("pi(%"PRIu32") = %"PRId32"\n", upper, ans);
    printf("Elapsed time: %f s\n", elapsed);

    return 0;
}

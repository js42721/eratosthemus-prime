#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sieve.h"
#include "types.h"

#define SIEVE_SIZE (1 << 18)

int main(int argc, char **argv)
{
    s32 ans;
    u32 upper;
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

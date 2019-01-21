#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "sieve.h"
#include "types.h"

#define SEGMENT_SIZE (1 << 18) /* [1, 2^29] */

void print(void *ctx, u64 prime)
{
    printf("%"PRIu64"\n", prime);
}

int main(int argc, char **argv)
{
    u64 lower;
    u64 upper;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s lower upper\n", argv[0]);
        return EXIT_FAILURE;
    }

    lower = upper = 0;

    sscanf(argv[1], "%"SCNu64, &lower);
    sscanf(argv[2], "%"SCNu64, &upper);

    sieve_generate(lower, upper, SEGMENT_SIZE, print, NULL);

    return 0;
}

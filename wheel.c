#include "wheel.h"

#define M0 ~(1 << 0)
#define M1 ~(1 << 1)
#define M2 ~(1 << 2)
#define M3 ~(1 << 3)
#define M4 ~(1 << 4)
#define M5 ~(1 << 5)
#define M6 ~(1 << 6)
#define M7 ~(1 << 7)

const u32 wheel[8] = { 1, 7, 11, 13, 17, 19, 23, 29 };

void init_prime(prime *p, u32 div, u32 mod, u32 step, u32 offset)
{
    p->div = div;
    p->mod = mod;
    p->step = step;
    p->offset = offset;
}

void mark_multiples(prime *p, u8 *sieve, u32 sieve_size)
{
    u32 i, i2, i4, i6;
    u32 interval;
    u32 limit;

    i = p->offset;
    if (i >= sieve_size)
        goto out;

    i2 = p->div * 2;
    i4 = i2 * 2;
    i6 = i4 + i2;

    interval = p->div * 30 + wheel[p->mod] - (i2 + 1);
    limit = (interval >= sieve_size) ? 0 : sieve_size - interval;

    switch (p->mod * 8 + p->step) {
    case 0:     do {    if (i >= sieve_size)
                            goto s0;
                        sieve[i] &= M0; i += i6;
    case 1:             if (i >= sieve_size)
                            goto s1;
                        sieve[i] &= M1; i += i4;
    case 2:             if (i >= sieve_size)
                            goto s2;
                        sieve[i] &= M2; i += i2;
    case 3:             if (i >= sieve_size)
                            goto s3;
                        sieve[i] &= M3; i += i4;
    case 4:             if (i >= sieve_size)
                            goto s4;
                        sieve[i] &= M4; i += i2;
    case 5:             if (i >= sieve_size)
                            goto s5;
                        sieve[i] &= M5; i += i4;
    case 6:             if (i >= sieve_size)
                            goto s6;
                        sieve[i] &= M6; i += i6;
    case 7:             if (i >= sieve_size)
                            goto s7;
                        sieve[i] &= M7; i += i2 + 1;

                        while (i < limit) {
                            sieve[i] &= M0; i += i6;
                            sieve[i] &= M1; i += i4;
                            sieve[i] &= M2; i += i2;
                            sieve[i] &= M3; i += i4;
                            sieve[i] &= M4; i += i2;
                            sieve[i] &= M5; i += i4;
                            sieve[i] &= M6; i += i6;
                            sieve[i] &= M7; i += i2 + 1;
                        }
                } while (1);

    case 8:     do {    if (i >= sieve_size)
                            goto s0;
                        sieve[i] &= M1; i += i6 + 1;
    case 9:             if (i >= sieve_size)
                            goto s1;
                        sieve[i] &= M5; i += i4 + 1;
    case 10:            if (i >= sieve_size)
                            goto s2;
                        sieve[i] &= M4; i += i2 + 1;
    case 11:            if (i >= sieve_size)
                            goto s3;
                        sieve[i] &= M0; i += i4;
    case 12:            if (i >= sieve_size)
                            goto s4;
                        sieve[i] &= M7; i += i2 + 1;
    case 13:            if (i >= sieve_size)
                            goto s5;
                        sieve[i] &= M3; i += i4 + 1;
    case 14:            if (i >= sieve_size)
                            goto s6;
                        sieve[i] &= M2; i += i6 + 1;
    case 15:            if (i >= sieve_size)
                            goto s7;
                        sieve[i] &= M6; i += i2 + 1;

                        while (i < limit) {
                            sieve[i] &= M1; i += i6 + 1;
                            sieve[i] &= M5; i += i4 + 1;
                            sieve[i] &= M4; i += i2 + 1;
                            sieve[i] &= M0; i += i4;
                            sieve[i] &= M7; i += i2 + 1;
                            sieve[i] &= M3; i += i4 + 1;
                            sieve[i] &= M2; i += i6 + 1;
                            sieve[i] &= M6; i += i2 + 1;
                        }
                } while (1);

    case 16:    do {    if (i >= sieve_size)
                            goto s0;
                        sieve[i] &= M2; i += i6 + 2;
    case 17:            if (i >= sieve_size)
                            goto s1;
                        sieve[i] &= M4; i += i4 + 2;
    case 18:            if (i >= sieve_size)
                            goto s2;
                        sieve[i] &= M0; i += i2;
    case 19:            if (i >= sieve_size)
                            goto s3;
                        sieve[i] &= M6; i += i4 + 2;
    case 20:            if (i >= sieve_size)
                            goto s4;
                        sieve[i] &= M1; i += i2;
    case 21:            if (i >= sieve_size)
                            goto s5;
                        sieve[i] &= M7; i += i4 + 2;
    case 22:            if (i >= sieve_size)
                            goto s6;
                        sieve[i] &= M3; i += i6 + 2;
    case 23:            if (i >= sieve_size)
                            goto s7;
                        sieve[i] &= M5; i += i2 + 1;

                        while (i < limit) {
                            sieve[i] &= M2; i += i6 + 2;
                            sieve[i] &= M4; i += i4 + 2;
                            sieve[i] &= M0; i += i2;
                            sieve[i] &= M6; i += i4 + 2;
                            sieve[i] &= M1; i += i2;
                            sieve[i] &= M7; i += i4 + 2;
                            sieve[i] &= M3; i += i6 + 2;
                            sieve[i] &= M5; i += i2 + 1;
                        }
                } while (1);

    case 24:    do {    if (i >= sieve_size)
                            goto s0;
                        sieve[i] &= M3; i += i6 + 3;
    case 25:            if (i >= sieve_size)
                            goto s1;
                        sieve[i] &= M0; i += i4 + 1;
    case 26:            if (i >= sieve_size)
                            goto s2;
                        sieve[i] &= M6; i += i2 + 1;
    case 27:            if (i >= sieve_size)
                            goto s3;
                        sieve[i] &= M5; i += i4 + 2;
    case 28:            if (i >= sieve_size)
                            goto s4;
                        sieve[i] &= M2; i += i2 + 1;
    case 29:            if (i >= sieve_size)
                            goto s5;
                        sieve[i] &= M1; i += i4 + 1;
    case 30:            if (i >= sieve_size)
                            goto s6;
                        sieve[i] &= M7; i += i6 + 3;
    case 31:            if (i >= sieve_size)
                            goto s7;
                        sieve[i] &= M4; i += i2 + 1;

                        while (i < limit) {
                            sieve[i] &= M3; i += i6 + 3;
                            sieve[i] &= M0; i += i4 + 1;
                            sieve[i] &= M6; i += i2 + 1;
                            sieve[i] &= M5; i += i4 + 2;
                            sieve[i] &= M2; i += i2 + 1;
                            sieve[i] &= M1; i += i4 + 1;
                            sieve[i] &= M7; i += i6 + 3;
                            sieve[i] &= M4; i += i2 + 1;
                        }
                } while (1);

    case 32:    do {    if (i >= sieve_size)
                            goto s0;
                        sieve[i] &= M4; i += i6 + 3;
    case 33:            if (i >= sieve_size)
                            goto s1;
                        sieve[i] &= M7; i += i4 + 3;
    case 34:            if (i >= sieve_size)
                            goto s2;
                        sieve[i] &= M1; i += i2 + 1;
    case 35:            if (i >= sieve_size)
                            goto s3;
                        sieve[i] &= M2; i += i4 + 2;
    case 36:            if (i >= sieve_size)
                            goto s4;
                        sieve[i] &= M5; i += i2 + 1;
    case 37:            if (i >= sieve_size)
                            goto s5;
                        sieve[i] &= M6; i += i4 + 3;
    case 38:            if (i >= sieve_size)
                            goto s6;
                        sieve[i] &= M0; i += i6 + 3;
    case 39:            if (i >= sieve_size)
                            goto s7;
                        sieve[i] &= M3; i += i2 + 1;

                        while (i < limit) {
                            sieve[i] &= M4; i += i6 + 3;
                            sieve[i] &= M7; i += i4 + 3;
                            sieve[i] &= M1; i += i2 + 1;
                            sieve[i] &= M2; i += i4 + 2;
                            sieve[i] &= M5; i += i2 + 1;
                            sieve[i] &= M6; i += i4 + 3;
                            sieve[i] &= M0; i += i6 + 3;
                            sieve[i] &= M3; i += i2 + 1;
                        }
                } while (1);

    case 40:    do {    if (i >= sieve_size)
                            goto s0;
                        sieve[i] &= M5; i += i6 + 4;
    case 41:            if (i >= sieve_size)
                            goto s1;
                        sieve[i] &= M3; i += i4 + 2;
    case 42:            if (i >= sieve_size)
                            goto s2;
                        sieve[i] &= M7; i += i2 + 2;
    case 43:            if (i >= sieve_size)
                            goto s3;
                        sieve[i] &= M1; i += i4 + 2;
    case 44:            if (i >= sieve_size)
                            goto s4;
                        sieve[i] &= M6; i += i2 + 2;
    case 45:            if (i >= sieve_size)
                            goto s5;
                        sieve[i] &= M0; i += i4 + 2;
    case 46:            if (i >= sieve_size)
                            goto s6;
                        sieve[i] &= M4; i += i6 + 4;
    case 47:            if (i >= sieve_size)
                            goto s7;
                        sieve[i] &= M2; i += i2 + 1;

                        while (i < limit) {
                            sieve[i] &= M5; i += i6 + 4;
                            sieve[i] &= M3; i += i4 + 2;
                            sieve[i] &= M7; i += i2 + 2;
                            sieve[i] &= M1; i += i4 + 2;
                            sieve[i] &= M6; i += i2 + 2;
                            sieve[i] &= M0; i += i4 + 2;
                            sieve[i] &= M4; i += i6 + 4;
                            sieve[i] &= M2; i += i2 + 1;
                        }
                } while (1);

    case 48:    do {    if (i >= sieve_size)
                            goto s0;
                        sieve[i] &= M6; i += i6 + 5;
    case 49:            if (i >= sieve_size)
                            goto s1;
                        sieve[i] &= M2; i += i4 + 3;
    case 50:            if (i >= sieve_size)
                            goto s2;
                        sieve[i] &= M3; i += i2 + 1;
    case 51:            if (i >= sieve_size)
                            goto s3;
                        sieve[i] &= M7; i += i4 + 4;
    case 52:            if (i >= sieve_size)
                            goto s4;
                        sieve[i] &= M0; i += i2 + 1;
    case 53:            if (i >= sieve_size)
                            goto s5;
                        sieve[i] &= M4; i += i4 + 3;
    case 54:            if (i >= sieve_size)
                            goto s6;
                        sieve[i] &= M5; i += i6 + 5;
    case 55:            if (i >= sieve_size)
                            goto s7;
                        sieve[i] &= M1; i += i2 + 1;

                        while (i < limit) {
                            sieve[i] &= M6; i += i6 + 5;
                            sieve[i] &= M2; i += i4 + 3;
                            sieve[i] &= M3; i += i2 + 1;
                            sieve[i] &= M7; i += i4 + 4;
                            sieve[i] &= M0; i += i2 + 1;
                            sieve[i] &= M4; i += i4 + 3;
                            sieve[i] &= M5; i += i6 + 5;
                            sieve[i] &= M1; i += i2 + 1;
                        }
                } while (1);

    case 56:    do {    if (i >= sieve_size)
                            goto s0;
                        sieve[i] &= M7; i += i6 + 6;
    case 57:            if (i >= sieve_size)
                            goto s1;
                        sieve[i] &= M6; i += i4 + 4;
    case 58:            if (i >= sieve_size)
                            goto s2;
                        sieve[i] &= M5; i += i2 + 2;
    case 59:            if (i >= sieve_size)
                            goto s3;
                        sieve[i] &= M4; i += i4 + 4;
    case 60:            if (i >= sieve_size)
                            goto s4;
                        sieve[i] &= M3; i += i2 + 2;
    case 61:            if (i >= sieve_size)
                            goto s5;
                        sieve[i] &= M2; i += i4 + 4;
    case 62:            if (i >= sieve_size)
                            goto s6;
                        sieve[i] &= M1; i += i6 + 6;
    case 63:            if (i >= sieve_size)
                            goto s7;
                        sieve[i] &= M0; i += i2 + 1;

                        while (i < limit) {
                            sieve[i] &= M7; i += i6 + 6;
                            sieve[i] &= M6; i += i4 + 4;
                            sieve[i] &= M5; i += i2 + 2;
                            sieve[i] &= M4; i += i4 + 4;
                            sieve[i] &= M3; i += i2 + 2;
                            sieve[i] &= M2; i += i4 + 4;
                            sieve[i] &= M1; i += i6 + 6;
                            sieve[i] &= M0; i += i2 + 1;
                        }
                } while (1);
    }

s0: p->step = 0; goto out;
s1: p->step = 1; goto out;
s2: p->step = 2; goto out;
s3: p->step = 3; goto out;
s4: p->step = 4; goto out;
s5: p->step = 5; goto out;
s6: p->step = 6; goto out;
s7: p->step = 7; goto out;
out:
    p->offset = i - sieve_size;
}

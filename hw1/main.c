#include <stdio.h>
#include <assert.h>
#include <memory.h>

void print_binary(unsigned int number) {
    if (number >> 1) {
        print_binary(number >> 1);
    }
    putc((number & 1) ? '1' : '0', stdout);
}

// 2.71
/* Declaration of data type where 4 bytes are packed
into an unsigned */
typedef unsigned packed_t;

/* Extract byte from word. Return as signed integer */
int xbyte(packed_t word, int bytenum) {
    return (signed) (word << ((3 - bytenum) << 3)) >> 24;
}

// 2.72
/* Copy integer into buffer if space is available */
/* WARNING: The following code is buggy */
void copy_int(int val, void *buf, int maxbytes) {
    if (maxbytes >= sizeof(val))
        memcpy(buf, (void *) &val, sizeof(val));
}

// 2.81
void generateBitPatternA(unsigned *pattern, int j, int k) {
    *pattern |= ~*pattern; // flip all 0s to 1s
    *pattern = *pattern << k;
}

void generateBitPatternB(unsigned *pattern, int j, int k) {
    *pattern |= ~*pattern; // flip all 0s to 1s
    *pattern = *pattern << (j + k);
    *pattern = *pattern | (j * 0xF) >> k;
    *pattern = ~*pattern;
}

// 2.90
//float fpwr2(int x) {
//    /* Result exponent and fraction */
//    unsigned exp, frac;
//    unsigned u;
//    if (x < -149) {
//        /* Too small. Return 0.0 */
//        exp = 0;
//        frac = 0;
//    } else if (x < -126) {
//        /* Denormalized result */
//        exp = 1;
//        frac = 1 << (x + 149);
//    } else if (x < 128) {
//        /* Normalized result. */
//        exp = x + 127;
//        frac = 0;
//    } else {
//        /* Too big. Return +oo */
//        exp = // 255;
//        frac = 0;
//    }
//    /* Pack exp and frac into 32 bits */
//    u = exp << 23 | frac;
//    /* Return as float */
//    return u2f(u);
//}

// 3.58
// x = %rdi, y = %rsi, z = %rdx, return = %rax
long decode2(long x, long y, long z) {
    y -= z;
    x *= y;
    long result = y;
    result <<= 63;
    result >>= 63;
    result ^= x;
    return result;
}

// 3.60
// x in %rdi, n in %esi
long loop(long x, long n) {
    long result = 0;
    long mask;
    for (mask = 1; mask != 0; mask <<= (n & 0xFF)) {
        result |= (x & mask);
    }
    return result;
}

int bitReverse(int x) {
    int reversed;
    //reversed = ((0xFF & x) << 24) + (((0xFF << 8) & x) << 8) + (((0xFF << 16) & x) >> 8) + (((0xFF << 24) & x) >> 24);
//    reversed =
//    ((x & 0x1 ) << 7) | ((x & 0x2 ) << 5) |
//    ((x & 0x4 ) << 3) | ((x & 0x8 ) << 1) |
//    ((x & 0x10) >> 1) | ((x & 0x20) >> 3) |
//    ((x & 0x40) >> 5) | ((x & 0x80) >> 7);



    int byteOne =
            ((x >> 24 & 0x1) << 7) |
            ((x >> 24 & 0x2) << 5) |
            ((x >> 24 & 0x4) << 3) |
            ((x >> 24 & 0x8) << 1) |
            ((x >> 24 & 0x10) >> 1) |
            ((x >> 24 & 0x20) >> 3) |
            ((x >> 24 & 0x40) >> 5) |
            ((x >> 24 & 0x80) >> 7);
    print_binary(x >> 24);
    printf("\n%s\n", "ByteOne: ");
    print_binary(byteOne);
    printf("\n");
    int byteTwo =
            ((x >> 16 & 0x1) << 7) |
            ((x >> 16 & 0x2) << 5) |
            ((x >> 16 & 0x4) << 3) |
            ((x >> 16 & 0x8) << 1) |
            ((x >> 16 & 0x10) >> 1) |
            ((x >> 16 & 0x20) >> 3) |
            ((x >> 16 & 0x40) >> 5) |
            ((x >> 16 & 0x80) >> 7);
    print_binary(x >> 16);
    printf("\n%s\n", "ByteTwo: ");
    print_binary(byteTwo);
    printf("\n");

    int byteThree =
            ((x >> 8 & 0x1) << 7) |
            ((x >> 8 & 0x2) << 5) |
            ((x >> 8 & 0x4) << 3) |
            ((x >> 8 & 0x8) << 1) |
            ((x >> 8 & 0x10) >> 1) |
            ((x >> 8 & 0x20) >> 3) |
            ((x >> 8 & 0x40) >> 5) |
            ((x >> 8 & 0x80) >> 7);
    print_binary(x >> 8);
    printf("\n%s\n", "ByteThree: ");
    print_binary(byteThree);
    printf("\n");

    int byteFour =
            ((x >> 0 & 0x1) << 7) |
            ((x >> 0 & 0x2) << 5) |
            ((x >> 0 & 0x4) << 3) |
            ((x >> 0 & 0x8) << 1) |
            ((x >> 0 & 0x10) >> 1) |
            ((x >> 0 & 0x20) >> 3) |
            ((x >> 0 & 0x40) >> 5) |
            ((x >> 0 & 0x80) >> 7);
    print_binary(x >> 0);
    printf("\n%s\n", "ByteFour: ");
    print_binary(byteFour);
    printf("\n");


    reversed = byteOne | byteTwo << 8 | byteThree << 16 | byteFour << 24;


    print_binary(reversed);
    printf("\n");
    return reversed;


}

unsigned floatUnsigned2Float(unsigned u) {
    if (u == 0) {
        return u;
    }

    // find decimal place movements required to have a leading zero
    int shift;
    for (shift = 0; u >> shift != 1; shift++);

    // add 127 for single precision bias
    int exponent = shift + 127;

    // get mantissa
    unsigned int mantissa = (~(0x1 << shift)) & u;

    int bitsToLose = shift - 23;
    if (bitsToLose < 0) {
        // losing no bits, shift is small
        int bitsToAdd = 23 - shift;
        // shift mantissa over to "add" bytes if needed
        mantissa <<= bitsToAdd;

    } else if (bitsToLose > 0) {
        // mask to save bits that will be lost when mantissa is shifted
        int lostBitMask = (0xFF >> (8 - bitsToLose));

        // mask to get MSB of bits being removed
        unsigned int deciderBitMask = (0x1 << (bitsToLose - 1));

        // check if it even makes sense to round up (bit to the right of the future LSB must be 1).
        unsigned int canRoundUp = (mantissa & deciderBitMask) >> (bitsToLose - 1);

        int shouldRoundUp = 0;
        if (canRoundUp) {
            unsigned int lostBits = (lostBitMask & mantissa);
            // if more than midpoint... else if less than midpoint...
            if (lostBits > (0x1 << (bitsToLose - 1))) {
                shouldRoundUp = 1;
            } else if (lostBits < (0x1 << (bitsToLose - 1))) {
                shouldRoundUp = 0;
            } else {
                // at a midpoint
                // check if rounding up will make LSB even. if so, 1 otherwise 0
                shouldRoundUp = !(0x1 & ((mantissa >> bitsToLose) + 1));
            }
        }

        // shift mantissa to get rid of "extra" bits
        mantissa >>= bitsToLose;
        // add one to mantissa to round up
        if (shouldRoundUp) mantissa++;
    }

    unsigned int result = (0x0 | exponent << 23) | (mantissa);
    return result;
}

int main() {

    // 2.71
//    printf("%d\n", xbyte(0x84030201, 3));
//    printf("%d\n", 0xFFFFFF84);

    // 2.81
    //unsigned patternA;
    //unsigned patternB = 0x84030201;
    //printf("%#04x\n", patternB);
    //generateBitPatternA(&patternA, 1, 6);
    //print_binary(patternA);
    //generateBitPatternB(&patternB, 4, 2);
    //printf("%#04x\n", patternB);
    //print_binary(patternB);

    // 2.82
    //int x = 0;
    //int y = 0;
    //printf("%d", ~x+~y+1==~(x+y));

    // 3.58
    //printf("%ld", decode2(100000, 3232323, 23123125));



    // 3.60
    //loop(3,4);
    //unsigned x = 0x80000000;
    //unsigned x = 0x80800000;
    //unsigned x = 0x3f800000;
    unsigned x = 0x3f800021;
    //unsigned x = 0xbf800000;
    //unsigned x = 0x800001;
    //unsigned x = 0x3f800000;
    //unsigned x = 0x7fffff;
    //unsigned x = 0x807fffff;
    //unsigned x = 0x807ffffe;
    //unsigned x = 0x3f800020;
    //unsigned x = 0x3f7fffe0;

    int y = 0x7fffffff;
    int z = 0x89ABCDEF;
    print_binary(x);
    printf("\n");
//    printf("\n");
//    int result = bitReverse(x);
//    //int result = z ^ ((z & 1) >> 4);
    unsigned int result = floatUnsigned2Float(x);
    print_binary(result);
    printf("\n");
    //printf("%d\n", result);
    printf("%#04x\n", result);
    //print_binary(result);
    //printf("\n");

    //printf("%#04x\n", floatUnsigned2Float(x));

    return 0;
}




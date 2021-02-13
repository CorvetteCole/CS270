/* 
 * CS:APP Data Lab 
 * 
 * Cole Gerdemann cjge227
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
    return ~(~x & ~y) & ~(x & y);
}

/*
 * bitOr - x|y using only ~ and & 
 *   Example: bitOr(6, 5) = 7
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitOr(int x, int y) {
    return ~(~x & ~y);
}

/*
 * isZero - returns 1 if x == 0, and 0 otherwise 
 *   Examples: isZero(5) = 0, isZero(0) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int isZero(int x) {
    return !x;
}
//2
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (least significant) to 3 (most significant)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
    return x >> (n << 3) & 0xFF;
}

/*
 * isNonNegative - return 1 if x >= 0, return 0 otherwise 
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNonNegative(int x) {
    return !((x >> 31) & 1);
}

/*
 * isNotEqual - return 0 if x == y, and 1 otherwise 
 *   Examples: isNotEqual(5,5) = 0, isNotEqual(4,5) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNotEqual(int x, int y) {
    return !!(x ^ y);
}

/*
 * oddBits - return word with all odd-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 2
 */
int oddBits(void) {
    return 0xAA + (0xAA << 8) + (0xAA << 16) + (0xAA << 24);
}

/*
 * copyLSB - set all bits of result to least significant bit of x
 *   Example: copyLSB(5) = 0xFFFFFFFF, copyLSB(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int copyLSB(int x) {
    return ~(x & 1) + 1;
}
//3
/* 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
    int result = x + y;
    return !(((result ^ x) & (result ^ y)) >> 31);
}

/*
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
    int byteIndex = n << 3; // essentially multiply by 8 to get byte index
    int result = ~(0xFF << byteIndex) & x; // copy x on to mask, leaving an empty byte filled with 0s
    return result | (c << byteIndex); // copy shifted replacement byte on to location
}

/*
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
    // if !x = 0 then y, if !x = 1 then z
    int mask = !x + ~0x00; // add result of !x to ~0x00 causing it to overflow to 0
    //  if z should be returned or stay at ~0x00 if y should be returned
    return (y & mask) | (z & ~mask);
}
//4
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
    return (((~x + 1) | x) >> 31) + 1;
}

/*
 * bitReverse - Reverse bits in a 32-bit word
 *   Examples: bitReverse(0x80000002) = 0x40000001
 *             bitReverse(0x89ABCDEF) = 0xF7D3D591
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 45
 *   Rating: 4
 */
int bitReverse(int x) {
    // yes, I definitely brute forced this one and it is over the ops limit.
    // I couldn't figure out how to do this in the ops restriction with such a low max constant (0xFF)

    const int xOne = x >> 24;
    const int xTwo = x >> 16;
    const int xThree = x >> 8;

    return
            (((xOne & 0x1) << 7) |
             ((xOne & 0x2) << 5) |
             ((xOne & 0x4) << 3) |
             ((xOne & 0x8) << 1) |
             ((xOne & 0x10) >> 1) |
             ((xOne & 0x20) >> 3) |
             ((xOne & 0x40) >> 5) |
             ((xOne & 0x80) >> 7)) |

            ((((xTwo & 0x1) << 7) |
              ((xTwo & 0x2) << 5) |
              ((xTwo & 0x4) << 3) |
              ((xTwo & 0x8) << 1) |
              ((xTwo & 0x10) >> 1) |
              ((xTwo & 0x20) >> 3) |
              ((xTwo & 0x40) >> 5) |
              ((xTwo & 0x80) >> 7)) << 8) |

            ((((xThree & 0x1) << 7) |
              ((xThree & 0x2) << 5) |
              ((xThree & 0x4) << 3) |
              ((xThree & 0x8) << 1) |
              ((xThree & 0x10) >> 1) |
              ((xThree & 0x20) >> 3) |
              ((xThree & 0x40) >> 5) |
              ((xThree & 0x80) >> 7)) << 16) |

            ((((x & 0x1) << 7) |
              ((x & 0x2) << 5) |
              ((x & 0x4) << 3) |
              ((x & 0x8) << 1) |
              ((x & 0x10) >> 1) |
              ((x & 0x20) >> 3) |
              ((x & 0x40) >> 5) |
              ((x & 0x80) >> 7)) << 24);
}

//float (4)
/* 
 * floatUnsigned2Float - Return bit-level equivalent of expression (float) u
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */

// I referenced https://en.wikipedia.org/wiki/Single-precision_floating-point_format while
// writing this function.

unsigned floatUnsigned2Float(unsigned u) {
    const int MANTISSA_SIZE = 23;
    const int EXPONENT_SIZE = 8;
    const int SINGLE_PRECISION_BIAS = 127;

    int shift;
    int exponent;
    int bitsToLose;

    unsigned int mantissa;

    if (u == 0) {
        return u;
    }

    // find decimal place movements required to have a leading zero
    for (shift = 0; u >> shift != 1; shift++);

    // add 127 for single precision bias
    exponent = shift + SINGLE_PRECISION_BIAS;

    // get mantissa
    mantissa = (~(0x1 << shift)) & u;

    bitsToLose = shift - MANTISSA_SIZE;
    if (bitsToLose <= 0) {
        // losing no bits, shift is small
        int bitsToAdd = MANTISSA_SIZE - shift;
        // shift mantissa over to "add" bytes if needed
        mantissa <<= bitsToAdd;

    } else {
        // mask to save bits that will be lost when mantissa is shifted
        int lostBitMask = (0xFF >> (EXPONENT_SIZE - bitsToLose));

        // mask to get MSB of bits being removed
        unsigned int deciderBitMask = (0x1 << (bitsToLose - 1));

        int shouldRoundUp;
        unsigned int lostBits = (lostBitMask & mantissa);
        // if more than midpoint... else if less than midpoint...
        if (lostBits > deciderBitMask) {
            shouldRoundUp = 1;
        } else if (lostBits < deciderBitMask) {
            shouldRoundUp = 0;
        } else {
            // at a midpoint
            // check if rounding up will make LSB even. if so, 1 otherwise 0
            shouldRoundUp = !(0x1 & ((mantissa >> bitsToLose) + 1));
        }

        // shift mantissa to get rid of "extra" bits
        mantissa >>= bitsToLose;
        // add one to mantissa to round up
        if (shouldRoundUp) mantissa++;
    }

    return (0x0 | exponent << MANTISSA_SIZE) | (mantissa);
}
// totals: 14 puzzles, 34 points

/* 
 * CS:APP Data Lab 
 * 
 * NAME:    Roxanne Lutz
 * USER ID: rlutz1
 * A#:      101990885 
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
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

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

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

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
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
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
/* Copyright (C) 1991-2022 Free Software Foundation, Inc.
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
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) { 
  // APPROACH 1:
  int shoveBits = x >> (n + (~0));
  int oneMoreShove = shoveBits >> 1; // potential issue with >> word size?
  return !(shoveBits ^ oneMoreShove);

  // printing for debug
  // printf("shove bits: %x\n", shoveBits);
  // printf("one more shove bits: %x\n", oneMoreShove);
  // printf("returning (0 for nope): %x\n", !(shoveBits ^ oneMoreShove));
  
  
  // APPROACH 2 (ILLEGAL): 
  // still fails on 0? but gets past the int min case
  // if (shoveBits == 0 || shoveBits == -1 ) { // or this, but extra illegal
  // if (!(shoveBits & 0xFFFFFFFF)) { // shoveBits is 0000...0000S
  //   // printf("1\n");
  //   return 1;
  // }

  // if (!((~shoveBits) & 0xFFFFFFFF)) {
  //    // shoveBits is 1111...1111
  //   // printf("1\n");
  //   return 1;
  // }

  // // printf("0\n");
  // return 0;
}
/* 
 * greatestBitPos - return a mask that marks the position of the
 *               most significant 1 bit. If x == 0, return 0
 *   Example: greatestBitPos(96) = 0x40
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 70
 *   Rating: 4 
 */
int greatestBitPos(int x) {

  x = ~x;

  // -------------------------------
  int last = x;
  int shift = x;

  int streak = 0;
  int allOnes = ~0; // for testing
  int flipflop = allOnes;
  
  int isAllOnes = 0; // 1 for yes, 0 for no when xor'd with a number
  int halfBits = 16;
  int currNum = 16;
 // can i make is all 1's 11111 if ture, 0 if false?

  // 16
  shift = x >> currNum; // shift by current number
  isAllOnes = !(shift ^ allOnes); // 1 if all ones, 0 if not
  // streak = streak + (currNum & (isAllOnes << 4)); // add num ONLY if all ones test returns as 1
  streak = streak + (isAllOnes << 4); // add 16 to streak if shift is all ones
  flipflop = allOnes + isAllOnes;
  last = (last & (~flipflop)) + (shift & flipflop);
  // last = (last & isAllOnes) + (shift & (~isAllOnes >> 1)); // if shift is nonzero, update last to that, otherwise, keep last

  // 8
  currNum = 8;
  shift = last << halfBits;
  shift = shift >> currNum;
  shift = shift >> halfBits;
  isAllOnes = !(shift ^ allOnes); // 1 if all ones, 0 if not
  // streak = streak + (currNum & (isAllOnes << currNum)); // add num ONLY if all ones test returns as 1
  streak = streak + (isAllOnes << 3);
  flipflop = allOnes + isAllOnes;
  last = (last & (~flipflop)) + (shift & flipflop);
  // last = (last & isAllOnes) + (shift & (~isAllOnes >> 1));
  
  // 4
  currNum = 4;
  shift = last << halfBits;
  shift = shift >> currNum;
  shift = shift >> halfBits;
  isAllOnes = !(shift ^ allOnes); // 1 if all ones, 0 if not
  // streak = streak + (currNum & (isAllOnes << currNum)); // add num ONLY if all ones test returns as 1
  streak = streak + (isAllOnes << 2);
  flipflop = allOnes + isAllOnes;
  last = (last & (~flipflop)) + (shift & flipflop);
  // last = (last & isAllOnes) + (shift & (~isAllOnes >> 1));

  // 2 
  currNum = 2;
  shift = last << halfBits;
  shift = shift >> currNum;
  shift = shift >> halfBits;
  isAllOnes = !(shift ^ allOnes); // 1 if all ones, 0 if not
  // streak = streak + (currNum & (isAllOnes << currNum)); // add num ONLY if all ones test returns as 1
  streak = streak + (isAllOnes << 1);
  flipflop = allOnes + isAllOnes;
  last = (last & (~flipflop)) + (shift & flipflop);
  // last = (last & isAllOnes) + (shift & (~isAllOnes >> 1));

  // 1
  currNum = 1;
  shift = last << halfBits;
  shift = shift >> currNum;
  shift = shift >> halfBits;
  isAllOnes = !(shift ^ allOnes); // 1 if all ones, 0 if not
  // streak = streak + (currNum & (isAllOnes << currNum)); // add num ONLY if all ones test returns as 1
  streak = streak + isAllOnes;
  flipflop = allOnes + isAllOnes;
  last = (last & (~flipflop)) + (shift & flipflop);
  // last = (last & isAllOnes) + (shift & (~isAllOnes >> 1));

  // edge case: if original all ones, add 1 to streak
  streak = streak + !(x ^ allOnes);
  // -------------------------------

  return 0x1 << (31 - streak); // soooooo close

}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  return 0;
}
/*
 * leftBitCount - returns count of number of consective 1's in
 *     left-hand (most significant) end of word.
 *   Examples: leftBitCount(-1) = 32, leftBitCount(0xFFF0F0F0) = 12
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Rating: 4
 * 
 * TODO: better more efficient way to test for all ones?
 * TODO: rid of the double >>, just hard code the numbers like a naughty girl.
 * down to 56 :)
 */
int leftBitCount(int x) {
  int last = x;
  int shift = x;

  int allOnes = ~0; // for testing
  int flipflop = 0; // logical flipping between last and new shift as needed

  int streak = !(x ^ allOnes); // starts at 1 if all ones, starts at 0 if not
  int isAllOnes = 0; // 1 for yes, 0 for no when xor'd with a number
  int halfBits = 16;
  int currNum = 16;


  // 16
  shift = x >> currNum; // shift by current number
  isAllOnes = !(shift ^ allOnes); // 1 if all ones, 0 if not
  // streak = streak + (currNum & (isAllOnes << 4)); // add num ONLY if all ones test returns as 1
  streak = streak + (isAllOnes << 4); // add 16 to streak if shift is all ones
  flipflop = allOnes + isAllOnes; // 1111 + 1 = 0, 1111 + 0 = 1111
  last = (last & (~flipflop)) + (shift & flipflop); // keep last if the shift was not all ones, otherwise remain
  // last = (last & isAllOnes) + (shift & (~isAllOnes >> 1)); // if shift is nonzero, update last to that, otherwise, keep last

  // 8
  currNum = 8;
  shift = last << halfBits;
  shift = shift >> 24; // 8 + 16 = 24
  // shift = shift >> halfBits;
  isAllOnes = !(shift ^ allOnes); // 1 if all ones, 0 if not
  // streak = streak + (currNum & (isAllOnes << currNum)); // add num ONLY if all ones test returns as 1
  streak = streak + (isAllOnes << 3);
  flipflop = allOnes + isAllOnes;
  last = (last & (~flipflop)) + (shift & flipflop);
  // last = (last & isAllOnes) + (shift & (~isAllOnes >> 1));
  
  // 4
  currNum = 4;
  shift = last << halfBits;
  shift = shift >> 20; // 4 + 16 = 20
  // shift = shift >> halfBits;
  isAllOnes = !(shift ^ allOnes); // 1 if all ones, 0 if not
  // streak = streak + (currNum & (isAllOnes << currNum)); // add num ONLY if all ones test returns as 1
  streak = streak + (isAllOnes << 2);
  flipflop = allOnes + isAllOnes;
  last = (last & (~flipflop)) + (shift & flipflop);
  // last = (last & isAllOnes) + (shift & (~isAllOnes >> 1));

  // 2 
  currNum = 2;
  shift = last << halfBits;
  shift = shift >> 18; // 2 + 16 = 18
  // shift = shift >> halfBits;
  isAllOnes = !(shift ^ allOnes); // 1 if all ones, 0 if not
  // streak = streak + (currNum & (isAllOnes << currNum)); // add num ONLY if all ones test returns as 1
  streak = streak + (isAllOnes << 1);
  flipflop = allOnes + isAllOnes;
  last = (last & (~flipflop)) + (shift & flipflop);
  // last = (last & isAllOnes) + (shift & (~isAllOnes >> 1));

  // 1
  currNum = 1;
  shift = last << halfBits;
  shift = shift >> 17; // 1 + 16 = 17
  // shift = shift >> halfBits;
  isAllOnes = !(shift ^ allOnes); // 1 if all ones, 0 if not
  // streak = streak + (currNum & (isAllOnes << currNum)); // add num ONLY if all ones test returns as 1
  streak = streak + isAllOnes;
  flipflop = allOnes + isAllOnes;
  last = (last & (~flipflop)) + (shift & flipflop);
  // last = (last & isAllOnes) + (shift & (~isAllOnes >> 1));

  return streak;
}
/*
 * satAdd - adds two numbers but when positive overflow occurs, returns
 *          maximum possible value, and when negative overflow occurs,
 *          it returns minimum positive value.
 *   Examples: satAdd(0x40000000,0x40000000) = 0x7fffffff
 *             satAdd(0x80000000,0xffffffff) = 0x80000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 30
 *   Rating: 4
 */
int satAdd(int x, int y) {
  return 2;
}
/*
 * satMul2 - multiplies by 2, saturating to Tmin or Tmax if overflow
 *   Examples: satMul2(0x30000000) = 0x60000000
 *             satMul2(0x40000000) = 0x7FFFFFFF (saturate to TMax)
 *             satMul2(0x60000000) = 0x80000000 (saturate to TMin)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int satMul2(int x) {
  return 2;
}
/*
 * satMul3 - multiplies by 3, saturating to Tmin or Tmax if overflow
 *  Examples: satMul3(0x10000000) = 0x30000000
 *            satMul3(0x30000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0x70000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0xD0000000) = 0x80000000 (Saturate to TMin)
 *            satMul3(0xA0000000) = 0x80000000 (Saturate to TMin)
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 3
 */
int satMul3(int x) {
    return 2;
}
/* 
 * float_half - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_half(unsigned uf) {
  return 2;
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  return 2;
}
/*
 * trueFiveEighths - multiplies by 5/8 rounding toward 0,
 *  avoiding errors due to overflow
 *  Examples: trueFiveEighths(11) = 6
 *            trueFiveEighths(-9) = -5
 *            trueFiveEighths(0x30000000) = 0x1E000000 (no overflow)
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 4
 */
int trueFiveEighths(int x)
{
    return 2;
}
/*
 * trueThreeFourths - multiplies by 3/4 rounding toward 0,
 *   avoiding errors due to overflow
 *   Examples: trueThreeFourths(11) = 8
 *             trueThreeFourths(-9) = -6
 *             trueThreeFourths(1073741824) = 805306368 (no overflow)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int trueThreeFourths(int x)
{
  int divByFour = x >> 2; // x * (1/(2^2))
  int intMax = 0x1 << 31; // int max: 1000..00
  int zeroNegOnePos = !(x & intMax); // returns 1 for positive nums, 0 for negs
  int lastTwoBits = x & 0x3; // grab the last two bits of any number
  int flipOne = !lastTwoBits; // 00 -> 1, all else -> 0
  int flipTwo = !flipOne; // // (00) 1 -> 0, (all else) 0 -> 1
  
  // 1 & pos (1) -> 1
  // 0 & pos (1) -> 0
  // 1, 0 & neg (0) -> 0
  int posAdjust = ~(flipTwo & zeroNegOnePos) + 1; // finally, make negative 1 as needed or remain zero
  int adjustment = lastTwoBits + posAdjust; // subtract 1 from last two bits as needed
  return divByFour + divByFour + divByFour + adjustment; // get three fourths with needed adjustment for rounding
}

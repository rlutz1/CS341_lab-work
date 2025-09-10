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
 * copyLSB - set all bits of result to least significant bit of x
 *   Example: copyLSB(5) = 0xFFFFFFFF, copyLSB(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int copyLSB(int x) {
   /*
    * @author Roxanne Lutz
    * implementation works as follows:
    * (1) and x with 0001 to carve out the LSBit. returns either 0001 or 0000
    * (2) not, creating either 1110 or 1111
    * (3) + 1, returning either 1111 or 0000
    */ 
  return ~(x & 1) + 1;
}
/* 
 * evenBits - return word with all even-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int evenBits(void) {
   /*
    * @author Roxanne Lutz
    * process is pretty brute-forcy. essentially: 
    * (1) init an 8 bit 01010101 to set the pattern
    * (2) shove it over 8 bits 4 times until the 32 bits reflect pattern
    */
   int cheat = 0x55; // 01010101 initialized
   int acc = cheat; // set accumulator to above
   acc = (acc << 8) + cheat; // 0101010101010101
   acc = (acc << 8) + cheat; // 010101010101010101010101
   acc = (acc << 8) + cheat; // 01010101...01010101
   acc = (acc << 8) + cheat; // 01010101.......01010101
   return acc;
}
/* 
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {
   /*
    * @author Roxanne Lutz
    * The main test is for a NaN which can be identified by
    * (1) all 1's in exp (bits 1 -> 9, left to right)
    * (2) non-zero value in the frac (10 -> 31, left to right)
    * note that we don't really care about the sign bit for this test.
    * 
    * so, approach is:
    * (1) test if uf is NaN with a mask
    * (2) if NaN, return immediately
    * (3) if not, turn the sign bit to 0 (& with 01111...1)
    */
   int NaNMask = 0xFF << 24; // 111111110000...0000
   int ufChopped = uf << 1; // "float" with sign bit removed for ease

   // if (1) all 1's in esp and (2) non-zero frac
   if ((ufChopped & NaNMask) == NaNMask && (ufChopped ^ NaNMask) != 0) {
      return uf; // NaN found, return unaltered
   } // end if

   // return "float" except for sign bit set to 0
   return uf & (~(0x80 << 24));
}
/* 
 * isTmin - returns 1 if x is the minimum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmin(int x) {
   /*
    * @author Roxanne Lutz
    * 
    * the method works with the following logic.
    * in 2's compliment arithmetic, only 2 numbers have the property below:
    * (num + 1) + (num - 1) = 0 
    * those 2 numbers are Tmin and 0.
    * to consistently ignore 0, we can simply !x -> 0 turns to one, all else to 0.
    * then add x such that 0 -> 1, all other x -> x.
    * we cannot subtract, so we have to add the binary rep of -1, ~0.
    * then, if (x - 1) + (x + 1) == 0 in only the Tmin case, we can 
    * return !(0) for 1 (Tmin), !(any other number) for 0 (not Tmin).
    * 
    * NOTE:
    * this method began with the attempt:
    * int num = (!x) + x; return !(num + num);
    * since (consider 4 bit int) 1000 + 1000 == 0000 due to truncation.
    * but, it seems that C has some unpredictable behavior with this overflow.
    * although printing that value was 0, the result was treated as nonzero,
    * so it returns instead 0 in the Tmin case. hence, the change to using boundary points.
    */

   int num = (!x) + x;
   int plusOne = num + 1;
   int minusOne = num + (~0);
   return !(plusOne + minusOne);

    // approach below also passes with same number of ops
   // int num = (!x) + x; // take care of the 0 case by always altering it to 1
   // int negOne = ~0; // use 1111..1 to get num get num - 1
   // int numMinusOne = (num + negOne); // subtract 1 from num
   // return !((numMinusOne + num) ^ negOne); // see above for logic on this
}

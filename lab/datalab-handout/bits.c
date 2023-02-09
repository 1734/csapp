/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
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
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  return ~(~(x & ~y) & ~(~x & y));
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 1 << 31;
}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  int a = !((x+1)^(~x));
  int b = !!(x+1);
  return a & b;
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  int a = 0xAA | (0xAA<<8);
  a = a | (a << 16);
  return !((x & a) ^ a);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return (~x)+1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  int a = !((x>>4) ^ 0x3);
  int b = (x>>3) & 0x01;
  int c = (x>>1) & 0x03;
  return a &  (!b | !c);
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  int a = !!x;
  int b = !x;
  return (y & (~a+1)) | (z & (~b+1));
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  int x_smallest = !(x ^ (1<<31));
  int x_neg = (x>>31) & 0x01;
  int y_neg = (y>>31) & 0x01;
  int neg_x = (~x) + 1;
  return x_smallest |
    (x_neg & !y_neg) |
    (((x_neg & y_neg) | !(x_neg | y_neg)) &
      !(((neg_x + y) >> 31) & 0x01));
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  return (((x | ((~x)+1)) >> 31) & 0x01) ^ 0x01;
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
  int neg1 = (~1) + 1;
  int mask16 = (1<<16) + neg1;
  int mask8 = 0xFF;
  int mask4 = 0x0F;
  int mask2 = 0x03;
  int mask1 = 0x01;

  int x32 = (x>>31) ^ x;
  int x32_high16 = (x32>>16);
  int x32_high16_zero = !x32_high16;
  int x32_16_shift = 16 & (x32_high16_zero + neg1);

  int x16 = (x32>>x32_16_shift) & mask16;
  int x16_high8 = (x16>>8);
  int x16_high8_zero = !x16_high8;
  int x16_8_shift = 8 & (x16_high8_zero + neg1);

  int x8 = (x16>>x16_8_shift) & mask8;
  int x8_high4 = (x8>>4);
  int x8_high4_zero = !x8_high4;
  int x8_4_shift = 4 & (x8_high4_zero + neg1);

  int x4 = (x8>>x8_4_shift) & mask4;
  int x4_high2 = (x4>>2);
  int x4_high2_zero = !x4_high2;
  int x4_2_shift = 2 & (x4_high2_zero + neg1);

  int x2 = (x4>>x4_2_shift) & mask2;
  int x2_high1 = (x2>>1);
  int x2_high1_zero = !x2_high1;
  int x2_1_shift = 1 & (x2_high1_zero + neg1);

  int x1 = (x2>>x2_1_shift) & mask1;
  int result = x1 + (!x2_high1_zero) + ((!x4_high2_zero) << 1) + ((!x8_high4_zero) << 2) + ((!x16_high8_zero) << 3) + ((!x32_high16_zero) << 4) + 1;

  return result;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  int mask_s = 0x80000000;
  int mask_exp = 0x7F800000;
  int mask_frac = 0x007FFFFF;
  int uf_exp = uf & mask_exp;
  int uf_frac = uf & mask_frac;
  int uf_s = uf & mask_s;
  if (uf_exp == mask_exp)
  {
    return uf;
  }
  if (uf_exp == 0) {
    if ((uf & 0x00400000) == 0x00400000) {
      return uf_s | 0x00800000 | ((uf_frac<<1)&mask_frac);
    }
    return uf_s | (uf_frac<<1);
  }
  if (uf_exp == 0x7F000000) {
    return uf_s | mask_exp;
  }
  return uf_s | (uf_exp + 0x00800000) | uf_frac;
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  unsigned mask_s = 0x80000000;
  unsigned mask_exp = 0x7F800000;
  unsigned mask_frac = 0x007FFFFF;
  unsigned uf_s = uf & mask_s;
  int uf_exp = uf & mask_exp;
  unsigned uf_frac = uf & mask_frac;
  int Bias = (1<<7) - 1;
  int E = (uf_exp>>23) - Bias;
  unsigned result_integer_part = 0;
  unsigned result_frac_part = 0;
  int result = 0;
  if (E >= 31) {
    return 0x80000000u;
  }
  if (E < 0) {
    return 0x0u;
  }
  if (E >= 23) {
    result_integer_part = (0x00800000 | uf_frac) << (E-23);
  } else {
    result_integer_part = (0x00800000 | uf_frac) >> (23-E);
  }
  result_frac_part = (uf_frac<<E) & mask_frac;
  if (result_frac_part > 0x00400000) {
    result_integer_part += 1;
  }
  else if (result_frac_part == 0x00400000 && (result_integer_part&0x01)) {
    result_integer_part += 1;
  }
  if (uf_s == 0) {
    result = result_integer_part;
  } else {
    result = (~result_integer_part) + 1;
  }
  return result;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
  unsigned pos_inf = 0x7F800000;
  unsigned k = 8;
  int n = 23;
  int Bias = (1<<(k-1)) -1;
  int e = 0;
  if (x > 127) {
    return pos_inf;
  }
  if (x < -126 - n) {
    return 0x0u;
  }
  if (x >= -126 && x <= 127) {
    e = x + Bias;
    return (e << n);
  }
  // x >= -126-n && x < -126
  return 0x00800000 >> (-x-126);
}

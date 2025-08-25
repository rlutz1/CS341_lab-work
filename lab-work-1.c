#include <stdio.h>

// same test case for all below right now.
int x = 0x874321;

/*
 * Write a function that yields 1 when run on a machine that uses arithmetic right shifts for data type
 * and yields  otherwise. Your code should work on a machine with any word size.
 * reminder that arithmetic right shift maintains the 1 bit as the MSB since two's complement.
 * approach 1: init a negative number; if we right shift and it's still negative, then arithmetic shifting
 */
int isArithmeticRight_V1() {
	signed int x = 0xF;
	printf("%#010X", x);
	int shift = x >> 1;
	printf("%#010X", shift);
	shift = x >> 2;
	printf("%#010X", shift);
	return shift < 0;
}

// second approach since i was a dirty little whore
// and i'm not actually supposed to use < ops
// both return false for this machine, which is good.
int isArithmeticRight_V2() {
	int x = 0xF;
	int shift = x >> 1;
	return shift & 0;
	// since x == 0xF, we can also:
	// return shift == x;
}

/*
 * write c expression that will yield a word consisting 
 * of the LSB of x and remaining bytes of y
 * exercise 2.59
 */
int lsbXRestY() {
	int x = 0x89ABCDEF;
	int y = 0x76543210;
	
	int mask = 0xFF; // 00000...11111111
	int lsbX = x & mask; // 0x000000EF
	
	mask = ~0xFF; // 11111...00000000
	int msbsY = y & mask; // 0x76543200
	
	return msbsY + lsbX; // 0x765432EF
} 

/*
 * last byte to all 1s, all other bytes unchanged
 * exercise 2.12
 */
int lsbOnesOtherwiseSame() {
	int mask = 0xFF;
	
	return x | mask;
}

/*
 * snag the last byte, complement everything else.  
 * exercise 2.12
 */
int lsbOtherwiseComplement() {
	int mask = 0xFF;
	
	return x ^ ~mask;
}

/*
 * snag the last byte, but nothing else.
 * exercise 2.12
 */
int lsbOtherwiseZero() {
	int mask = 0xFF;
	
	return x & mask;
}

/*
 * run everything baby doll
 */
int main() {
	printf("Test case: ");
	printf("%#010X", + x);
	printf("\n");
	
	int returnVal = lsbOtherwiseZero();
	printf("lsbOtherwiseZero: ");
	printf("%#010X", + returnVal);
	printf("\n");
	
	returnVal = lsbOtherwiseComplement();
	printf("lsbOtherwiseComplement: ");
	printf("%#010X", returnVal);
	printf("\n");
	
	returnVal = lsbOnesOtherwiseSame();
	printf("lsbOnesOtherwiseSame: ");
	printf("%#010X", returnVal);
	printf("\n");
	
	returnVal = lsbXRestY();
	printf("lsbXRestY: ");
	printf("%#010X", returnVal);
	printf("\n");
	
	returnVal = isArithmeticRight_V2();
	printf("isArithmeticRight: ");
	if (returnVal != 0) {
		printf("true");
	} else {
		printf("false");
	} // end if
	printf("\n");
	
	return 0;
}


#include <stdio.h>

// same test case for all below right now.
int x = 0x874321;

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
	int msbsY = y & mask;
	
	return msbsY + lsbX;
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
	
	return 0;
}


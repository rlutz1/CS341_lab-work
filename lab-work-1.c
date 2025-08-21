#include <stdio.h>

/*
 * snag the last byte, complement everything else.  
 */
int lsbOtherwiseComplement() {
	int x = 0x874321;
	int mask = 0xFF;
	
	return x ^ ~mask;
}

/*
 * snag the last byte, but nothing else.  
 */
int lsbOtherwiseZero() {
	int x = 0x874321;
	int mask = 0xFF;
	
	return x & mask;
}

int main() {
	int returnVal = lsbOtherwiseZero();
	printf("%#010X", returnVal);
	return 0;
}


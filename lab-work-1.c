#include <stdio.h>

/*
 * snag the 
 *
 */
int lsbOtherwiseZero() {
	int x = 0x874321;
	int mask = 0xFF;
	
	return mask;
}

int main() {
	int returnVal = lsbOtherwiseZero();
	printf("%#010X", returnVal);
	return 0;
}


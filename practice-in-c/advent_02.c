#include <stdio.h>
#include <stdlib.h>
/**
 * find if the reports are safe:
 * 1. col values are either all increasing/all decreasing
 * 2. any 2 adj cols differ by at least 1 && at most 3
 * https://adventofcode.com/2024/day/2
 * 
 * APPROACH 1:
 * simple as, 
 * (1) take in the input and turn it into a linked list of linked lists (see side project)
 * (2) iterate through each row and check the conditions using a state management system
 * (3) return the number safe
 */

int main(int argc, char *argv[]) {
  char *input = getLine(); // https://man7.org/linux/man-pages/man3/getline.3.html
  // scanf("%[^\n]s", input); 

  printf("%s\n", input);
  return 0;  
} // end main method


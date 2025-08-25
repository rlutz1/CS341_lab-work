# include <stdio.h>

/*
 * the setup is basically this:
 * a lot of talk, but essentially, we have two lists of numbers.
 * we want to pair up the numbers with the smallest in order.
 * then sum all the differences.
 * we'll assume that the lists are not guaranteed to be the same length.
 * https://adventofcode.com/2024/day/1
 * 
 * APPROACH 1:
 * simple as, 
 * (1) sort the two lists
 * (2) iterate through and tally the differences between the points
 */

int main(int argc, char *argv[]) {

  for (int i = 1; i < argc; i++) { // note that i = 0 prints the exe file path
    printf("entered: %s\n", argv[i]);
  } // end loop

  return 0;
} // end main method
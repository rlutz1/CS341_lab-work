# include <stdio.h>

/**
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

int sort();
int *parseList();

int main(int argc, char *argv[]) {

  if (argc < 3) { return 2; } // not enough arg, quick check

  // we're gonna assume that the lists are passed in format
  // "1, 2, 3" => single string, vals separated by commas
  // no error checking will be done here since i'm the only one using, lol
  
  int *list1 = parseList(argv[1]);
  int *list2 = parseList(argv[2]);

  for (int i = 1; i < argc; i++) { // note that i = 0 prints the exe file path
    printf("entered: %s\n", argv[i]);
  } // end loop

  free(list1);
  free(list2);

  return 0;
} // end main method

/** 
 * 
 */
int *parseList() {

} // end method

int sort() {
  return 0;
} // end method


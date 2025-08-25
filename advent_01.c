#include <stdio.h>
#include <stdlib.h>
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
int freeStrings(char **strs, int numStrs);
char **split(char *str, char delim);

int main(int argc, char *argv[]) {

  if (argc < 3) { return 2; } // not enough arg, quick check

  // we're gonna assume that the lists are passed in format
  // "1, 2, 3" => single string, vals separated by commas
  // no error checking will be done here since i'm the only one using, lol
  
  char **strs1 = split(argv[1], ',');
  
  char **strs2 = split(argv[2], ',');


  freeStrings(strs1, 3);
  freeStrings(strs2, 4);
  // for (int i = 1; i < argc; i++) { // note that i = 0 prints the exe file path
  //   printf("entered: %s\n", argv[i]);
  // } // end loop

  // free(list1);
  // free(list2);

  return 0;
} // end main method

/** 
 * (1) take in a string with expected "1, 2, 3" 
 * (2) split up on the ','
 * (3) return the allocated int *
 */
char **split(char *str, char delim) {
  // int length = sizeof(str) / sizeof(*str);
  
  // printf("%d\n", length);
  // printf("%s")

  // for (int i = 0; i < str)
  int i = 0; int nums = 0;

  char *temp = malloc(10 * sizeof(char)); // should just be 1 (byte), can have up to 10 digits in int
  if (temp == NULL) { return NULL; }
  int tempCounter = 0;

  char **numStrings = malloc(10 * sizeof(temp)); // TODO: hardcoded 10 num strings
  if (numStrings == NULL) { return NULL; }
  int numStringsCounter = 0;

  while (*(str + i) != '\0') { // while not null terminating character
    
    if (*(str + i) == delim) { // if we find the delim
      if (tempCounter > 0) { // if we have a non empty number
        // add it to the other list
        *(numStrings + numStringsCounter) = temp;
        numStringsCounter++;
        temp = malloc(10 * sizeof(char));
        if (temp == NULL) { return NULL; }
        tempCounter = 0;
        printf("Adding a number: %s\n", *(numStrings + numStringsCounter - 1));
      } // end if
    } else {
      if (*(str + i) != ' ') {
        *(temp + tempCounter) = *(str + i);
        tempCounter++;
        // printf("Adding to temp.");
        printf("Adding to temp: %c\n", *(temp + tempCounter - 1));
      }
      
    }
    i++;
  } // end loop

  if (tempCounter > 0) { // if we have a non empty number
    // add it to the other list
    *(numStrings + numStringsCounter) = temp;
    printf("Adding a final number: %s\n", *(numStrings + numStringsCounter));
  } // end if

  if (numStringsCounter < 10) { numStrings = realloc(numStrings, numStringsCounter * sizeof(temp)); }
  return numStrings;
} // end method

int sort() {
  return 0;
} // end method

int freeStrings(char **strs, int numStrs) {
  int length = sizeof(strs) / sizeof(char *);
  printf("Length: %d\n", length);
  // printf("size of strs: %d\n", sizeof(strs));
  // printf("sizeof *strs: %d\n", sizeof(*strs));
  for(int i = 0; i < numStrs; i++) {
    printf("Freeing: %s\n", *(strs + i));
    free(*(strs + i));
  } // end loop
  printf("Freeing pointer.\n");
  free(strs);
  return 0;
}
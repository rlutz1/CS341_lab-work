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

int sort(int *nums, int length);
int freeListStr(char **strs, int length);
int freeListInt(int *nums, int length);
char **split(char *str, char delim, int length);
int *parseInt(char ** strs, int length);

int main(int argc, char *argv[]) {

  if (argc < 5) { return 2; } // not enough arg, quick check

  // we're gonna assume that the lists are passed in format
  // "1, 2, 3" => single string, vals separated by commas
  // no error checking will be done here since i'm the only one using, lol
  
  int length1 = atoi(argv[2]);
  if (length1 == 0) { return 1; }

  int length2 = atoi(argv[4]);
  if (length2 == 0) { return 1; }

  char **strs1 = split(argv[1], ',', length1);
  char **strs2 = split(argv[3], ',', length2);

  int *ints1 = parseInt(strs1, length1);
  freeListStr(strs1, length1);
  if (ints1 == NULL) { return 2; }

  // printf("INTS 1: \n");
  // for (int i = 0; i < length1; i++) {
  //   printf("%d | ", *(ints1 + i));
  // } // end loop
  // printf("\n");


  int *ints2 = parseInt(strs2, length2);
  freeListStr(strs2, length2);
  if (ints2 == NULL) { return 2; }
  
  // printf("INTS 2: \n");
  // for (int i = 0; i < length2; i++) {
  //   printf("%d | ", *(ints2 + i));
  // } // end loop
  // printf("\n");
  
  sort(ints1, length1);
  // sort(ints2, length2); // TODO: uncomment

  freeListInt(ints1, length1);
  freeListInt(ints2, length2);



  return 0;
} // end main method

/** 
 * (1) take in a string with expected "1, 2, 3" 
 * (2) split up on the ','
 * (3) return the allocated char **
 * 
 * heads up that this is BY NO MEANS thoroughly error checked.
 */
char **split(char *str, char delim, int length) {
  
  char *num = malloc(10 * sizeof(char)); // should just be 1 (byte), can have up to 10 digits in int
  if (num == NULL) { return NULL; } // null pointer check
  int numCounter = 0; // init temp string counter

  char **numStrings = malloc(length * sizeof(num)); // TODO: hardcoded 10 num strings
  if (numStrings == NULL) { return NULL; } // null pointer check
  int numStringsCounter = 0; // number of strings, for indexing

  int i = 0;
  while (*(str + i) != '\0') { // while not null terminating character
    
    if (*(str + i) == delim) { // if we find the delim
      if (numCounter > 0) { // if we have a non empty number
        // add it to the list of strings
        *(numStrings + numStringsCounter) = num;
        numStringsCounter++;
        num = malloc(10 * sizeof(char)); // set aside more memory for next number
        if (num == NULL) { return NULL; } // null pointer check
        numCounter = 0; // reset the temp num counter
        printf("Added a number: %s\n", *(numStrings + numStringsCounter - 1));
      } // end if
    } else {
      if (*(str + i) != ' ') { // ignore whitespaces
        *(num + numCounter) = *(str + i); // add the character to the temp
        numCounter++;
        // printf("Adding to temp.");
        printf("Added to temp: %c\n", *(num + numCounter - 1));
      } // end if
      
    }
    i++; // increment to next char regardless
  } // end loop

  if (numCounter > 0) { // if we have a non empty number, final dump
    // add it to the other list
    *(numStrings + numStringsCounter) = num;
    numStringsCounter++; // not necessary logically, but for error printing.
    printf("Added to temp: %s\n", *(numStrings + numStringsCounter - 1));
  } // end if

  // reallocate the size as needed.
  // if (numStringsCounter < 10) { numStrings = realloc(numStrings, numStringsCounter * sizeof(num)); }
  if (numStringsCounter != length) { printf("Processed %d numbers, but was told there are %d numbers.\n", numStringsCounter, length); }
  return numStrings;
} // end method

int sort(int *nums, int length) {
  
  return 0;
} // end method

/**
 * turn a list of strings
 * into a list of ints.
 * return null if anything goes wrong.
 */
int *parseInt(char **strs, int length) {

  int *nums = malloc(length * sizeof(int));
  int num;

  for (int i = 0; i < length; i++) {
    num = atoi(*(strs + i));
    if (num == 0 && **(strs + i) != '0') { // error catching for atoi
      free(nums);
      return NULL;
    } // end if

    // if a valid number was converted, add it to nums
    *(nums + i) = num;
  } // end loop

  return nums;
} // end method

int freeListStr(char **elements, int length) {
  // int length = sizeof(strs) / sizeof(char *);
  // printf("Length: %d\n", length);

  for(int i = 0; i < length; i++) {
    printf("Freeing: %s\n", *(elements + i));
    free(*(elements + i));
  } // end loop
  printf("Freeing pointer.\n");
  free(elements);
  return 0;
} // end method

int freeListInt(int *elements, int length) {
  // int length = sizeof(strs) / sizeof(char *);
  // printf("Length: %d\n", length);

  // for(int i = 0; i < length; i++) {
  //   printf("Freeing: %d\n", *(elements + i));
  //   free(*(elements + i));
  // } // end loop
  printf("Freeing pointer.\n");
  free(elements);
  return 0;
} // end method
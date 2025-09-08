# include <stdio.h>

/*
 * just a quick illustration of the c
 * main method signature when handing off arguments
 * on the command line. 
 * for reference only.
 */

int main(int argc, char *argv[]) {

  for (int i = 1; i < argc; i++) { // note that i = 0 prints the exe file path
    printf("entered: %s\n", argv[i]);
  } // end loop

  return 0;
} // end main method
#include "bits.c"
#include <stdio.h>

int testFitsBits () {
  int intMin = 0x80000000;
  int intMax = 0x7FFFFFFF;

  for (int x = intMin; x <= 0; x++) {
    // printf("%d\n", x);
    int neededBits = howManyBits(x);
    for (int n = neededBits; n <= 32; n++) {
      if (!fitsBits(x, n)) {
        printf("Failed for %x on %d bits.\n", x, n);
      }
    }

    for (int n = 1; n < neededBits; n++) {
      if (fitsBits(x, n)) {
        printf("Failed for %x on %d bits.\n", x, n);
      }
    }
  }

  printf("Done with negatives.\n");

  for (int x = 1; x <= intMax; x++) {
    // printf("%d\n", x);
    int neededBits = howManyBits(x);
    for (int n = neededBits; n <= 32; n++) {
      if (!fitsBits(x, n)) {
        printf("Failed for %x on %d bits.\n", x, n);
      }
    }

    for (int n = 1; n < neededBits; n++) {
      if (fitsBits(x, n)) {
        printf("Failed for %x on %d bits.\n", x, n);
      }
    }
  }

  printf("Done with positives.\n");
}

int main() {
  testFitsBits();
  return 0;
}
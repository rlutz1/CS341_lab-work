#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>  
#include <stdio.h>

#define MAX_LINE_SIZE 256

// struct definitions for ease of reading
typedef struct  {
    char *tag; // not sure if this is most appropriate yet; could be better with nuuummbberrr?? since tag is unique, decimal number from it should be as well?
    char valid; // 0 or 1
    short priority; // for mimicking LRU functionality. likely never more than short
    short numBlocks;
    char *data;
} Line;

typedef struct {
    int numLines; // added because C is complaining with it, boo
    Line *lines;
} Set;

typedef struct {
    int numSets;
    Set *sets;
} Cache;

// globals
static char *traceFilename = 0;
static int hits = 0;
static int misses = 0;
static int evictions = 0;

// method declarations
Cache initCache(int argc, char *argv[]);
void simulate();
char *convertHexToBinary(char hex);
void getAddressConversion(char *line, char *binaryAddress);
void tryReadCache(void *addr);
void tryWriteCache(void *addr);
void checkNullPtr(void *ptr);
void freeAllMemory(Cache cache);
void printCache(Cache cache);

int main(int argc, char *argv[])
{    // malloc you fucking idiot
    // (1) extract all arguments and values 
    Cache cache = initCache(argc, argv);
    printCache(cache);

    // (2) go through lines and attempt to interact through cache
    // -- if L: tryReadCache(addr)
    // -- if S: tryWriteCache(addr) // kinda the same as L really
    // -- if M: tryReadCache(addr), tryWriteCache(addr) // load then store
    // ** COUNT HITS AND MISSES AND EVICTIONS!
    simulate();

    printSummary(hits, misses, evictions);
    freeAllMemory(cache);
    return 0;
}

void simulate() {
    if (traceFilename) {
        FILE *fp;
        char line[MAX_LINE_SIZE];

        fp = fopen(traceFilename, "r");

        if (!fp) {
            printf("Something went wrong: file open failed.\n"); return;
        } // end if

        while (fgets(line, sizeof(line), fp)) {
            // printf("%s", line);

            if (line[0] == ' ') { // ignore all I instructions
                printf("%s", line);
                // we need to (1) snag the argument
                char action = line[1];
                printf("%c\n", action);
                // (2) grab the hex address and convert to binary
                char binaryAddress[64] = {0};
                getAddressConversion(line, binaryAddress);
                printf("%s\n", binaryAddress);
                // we can ignore the size here
                // free(binaryAddress);
            }
                

        } // end loop

        fclose(fp);

    } else {
        printf("Something went wrong: trace file name was not given.\n");
    } // end if
} // end method

/**
 * purpose of this method is to grab the hex address and convert to 
 * binary. return it as a string of binary numbers.
 * number of bits is at least 64
 */
void getAddressConversion(char *line, char *binaryAddress) {
    // char *binaryAddress = (char *) malloc(64 * sizeof(char)); // 64 bits
    char hex;
    char *hexToBinary;

    char startIndex = 3;
    // char endIndex = 11; // characters 3 - 10 are the address
    char binaryIndex = 0;
   
    char countHex = 0;
    short index = 3;

    while (line[index] != ',') {
        countHex += 1;
        index += 1;
    }

    char fillerZeros = (16 - countHex) * 4; // have 2, expecting 8, that means 6 filler zeros
    for (int i = 0; i < fillerZeros; i++) {
        binaryAddress[i] = '0';
    } // end loop

    binaryIndex = fillerZeros;
    printf("filler zeros: %s\n", binaryAddress);

    // for (int i = startIndex; i < endIndex; i++) {
    int i = startIndex;
    while ((hex = line[i]) != ',') {

        hexToBinary = convertHexToBinary(hex); // get the binary of this hex char
        if (!hexToBinary) // don't want to continue if something went awry there
            break;

        for (int j = 0; j < 4; j++) 
            binaryAddress[binaryIndex + j] = hexToBinary[j];
                
        binaryIndex += 4;
        printf("after adding %c -> %s:\n%s\n",hex, hexToBinary, binaryAddress);
        i++;
    }
    // return binaryAddress;
} // end method

/**
 * super quick and dirty hex conversion
 */
char * convertHexToBinary(char hex) {
    switch (hex) { // gonna do a switch case because i'm very naughty
            case '0':
                return "0000"; break;
            case '1':
                return "0001"; break;
            case '2':
                return "0010"; break;
            case '3':
                return "0011"; break;
            case '4':
                return "0100"; break;
            case '5':
                return "0101"; break;
            case '6':
                return "0110"; break;
            case '7':
                return "0111"; break;
            case '8':
                return "1000"; break;
            case '9':
                return "1001"; break;
            case 'A':
            case 'a':
                return "1010"; break;
            case 'B':
            case 'b':
                return "1011"; break;
            case 'C':
            case 'c':
                return "1100"; break;
            case 'D':
            case 'd':
                return "1101"; break;
            case 'E':
            case 'e':
                return "1110"; break;
            case 'F':
            case 'f':
                return "1111"; break;
                
        } // end return case;
        printf("Something went wrong: received an undefined hex value: %c", hex);
        return 0;
} // end method

/**
 * initialize the cache based on arguments passed.
 */
Cache initCache(int argc, char *argv[]) {
    int opt;
    int numSets = 0;
    int numSetIndexBits = 0;
    int numLines = 0;
    int numBlocks = 0;
    int numBlockBits = 0;
    
    // use getopt to easily extract arguments
    while((opt = getopt(argc, argv, "s:E:b:t:")) != -1) 
    { 

        switch(opt) 
        { 
            case 's':
                numSetIndexBits = atoi(optarg);
                numSets = pow(2, numSetIndexBits);
                break;
            case 'E': 
                numLines = atoi(optarg);
                break;
            case 'b':
                numBlockBits = atoi(optarg);
                numBlocks = pow(2, numBlockBits);
                break;
            case 't':
                traceFilename = optarg;
                break;
            case ':': 
                printf("option needs a value\n"); 
                break; 
            case '?': 
                printf("unknown option: %c\n", optopt);
                break; 
        } // end switch case
    }  // end loop

    // initialize the cache based on passed paramaters
    Set *sets = (Set *) malloc(numSets * sizeof(Set));
    checkNullPtr(sets);
    for (int i = 0; i < numSets; i++) {

        Line *lines = (Line *) malloc(numLines * sizeof(Line));
        checkNullPtr(lines);

        for (int j = 0; j < numLines; j++) {
            char *blocks = (char *) malloc(numBlocks * sizeof(char));
            checkNullPtr(blocks);

            Line line = {0, 0, -1, numBlocks, blocks}; // maybe fine?
            lines[j] = line;
          //  printf("set %d line %d", i, j);
        } // end loop

        Set set = {numLines, lines};
        sets[i] = set;
    } // end loop

    Cache cache = {numSets, sets};
    return cache;
} // end method

void checkNullPtr(void *ptr) {
    if (!ptr) {
        printf("Null pointer returned by malloc.\n");
        exit(2);
    } // end if
} // end method

/**
 * need to ensure freeing of all malloc'd memory upon program exit:
 * + sets
 * + lines
 * + blocks
 */
void freeAllMemory(Cache cache) {
    printf("Freeing memory... \n"); 

    for (int i = 0; i < cache.numSets; i++) {

        Set set = cache.sets[i];
        Line *lines = set.lines;
        for (int j = 0; j < set.numLines; j++) {
            // Line line = set.lines[j];
            Line line = lines[j];
            free(line.data);
        } // end loop
        free(lines);
    }
    free(cache.sets);
} // end method

/**
 * method for printing cache for debugging purposes only.
 */
void printCache(Cache cache) {
    printf("Number of sets: %d \n\n", cache.numSets); 

    for (int i = 0; i < cache.numSets; i++) {

        Set set = cache.sets[i];
        Line *lines = set.lines;
        printf("Num lines in set %d: %d \n", i, set.numLines);
        for (int j = 0; j < set.numLines; j++) {
  
            Line line = lines[j];
   
            printf("\nLINE %d \n", j);
            printf("tag in line: %s \n", line.tag);
            printf("valid bit: %d \n", line.valid);
            printf("priority: %d \n", line.priority);
            printf("number of bytes of data in line: %d \n", line.numBlocks);
            printf("data in blocks of line: %s \n", line.data);
            printf("END LINE\n\n");
        } // end loop
    } // end loop
} // end method

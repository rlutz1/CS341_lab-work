#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>  
#include <stdio.h>

#define MAX_LINE_SIZE 256
#define NUM_HEX_IN_ADDRESS 16
#define NUM_BITS_IN_ADDRESS 64
#define SPACE_INDEX 0 
#define ACTION_INDEX 1
#define ADDRESS_START_INDEX 3

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
    int numTagBits;
    int numSetBits;
    int numBlockBits;
    Set *sets;
} Cache;

// globals
static char *traceFilename = 0;
static int hits = 0;
static int misses = 0;
static int evictions = 0;

// method declarations
Cache initCache(int argc, char *argv[]);
void simulate(Cache cache);
void hit();
void miss();
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
    // printCache(cache);

    // (2) go through lines and attempt to interact through cache
    // -- if L: tryReadCache(addr)
    // -- if S: tryWriteCache(addr) // kinda the same as L really
    // -- if M: tryReadCache(addr), tryWriteCache(addr) // load then store
    // ** COUNT HITS AND MISSES AND EVICTIONS!
    simulate(cache);

    printSummary(hits, misses, evictions);
    freeAllMemory(cache);
    return 0;
}

void simulate(Cache cache) {
    if (traceFilename) {
        FILE *fp;
        char line[MAX_LINE_SIZE];

        fp = fopen(traceFilename, "r");

        if (!fp) {
            printf("Something went wrong: file open failed.\n"); return;
        } // end if

        while (fgets(line, sizeof(line), fp)) {
            // printf("%s", line);

            if (line[SPACE_INDEX] == ' ') { // ignore all I instructions
                printf("%s", line);
                // we need to (1) snag the argument
                char action = line[ACTION_INDEX];
                printf("%c\n", action);
                // (2) grab the hex address and convert to binary
                char binaryAddress[NUM_BITS_IN_ADDRESS] = {0};
                getAddressConversion(line, binaryAddress);
                printf("%s\n", binaryAddress);
                // we can ignore the size here

                // okay, so we have the binary rep now in binaryAddress
                // we need the cache to know how many bits for set and block
                // m - (s + b) is the tag, held in cache
                char tag[cache.numTagBits];

                // for (int i = 0; i < cache.numTagBits; i++) {
                //     tag[i] = 0;
                // } // end loop
            
                for (int i = 0; i < cache.numTagBits; i++) {
                    tag[i] = binaryAddress[i];
                } // end loop
                tag[cache.numTagBits] = '\0';

                char setNum[cache.numSetBits];
                for (int i = cache.numTagBits, j = 0; j < cache.numSetBits; i++, j++) {
                    setNum[j] = binaryAddress[i];
                } // end loop
                setNum[cache.numSetBits] = '\0';

                printf("tag: %s\nsetnum: %s\n", tag, setNum);
                // we can snag set bits and convert that to decimal
                // then we need tag
                // we look at cache.sets[set decimal]
                // we look at each line (or technically until priority == -1 due to heaping) for the tag. for now, let's look at all
                // if (tag == tag): hit count++; hit() // conduct hit procedure
                // if we look through all lines an no dice: missCount++ miss()  // call miss procedure 
            }
                

        } // end loop

        fclose(fp);

    } else {
        printf("Something went wrong: trace file name was not given.\n");
    } // end if
} // end method


void hit() {

}

void miss() {
    // need to increment eviction here if that is needed
}

/**
 * purpose of this method is to grab the hex address and convert to 
 * binary. return it as a string of binary numbers.
 * number of bits is at least 64
 */
void getAddressConversion(char *line, char *binaryAddress) {
    char hex;
    char *hexToBinary;
    char binaryIndex = 0; // index for filling binary rep of address
    short lineIndex = ADDRESS_START_INDEX; // start of the address in line
    char countHex = 0; // for counting how many hex given

    // count how many hex chars there are in given address
    while (line[lineIndex] != ',') {
        countHex++;
        lineIndex++;
    } // end loop

    lineIndex = ADDRESS_START_INDEX; // reset

    // calculate how many filler zeros needed at start of binary address
    char fillerZeros = (NUM_HEX_IN_ADDRESS - countHex) * 4;
    for (short i = 0; i < fillerZeros; i++) { // fill with filler zeros
        binaryAddress[i] = '0';
    } // end loop

    binaryIndex = fillerZeros; // offset the index by num filler zeros
    // printf("filler zeros: %s\n", binaryAddress);

    while ((hex = line[lineIndex]) != ',') { 

        hexToBinary = convertHexToBinary(hex); // get the binary of this hex char
        if (!hexToBinary) // don't want to continue if something went awry there
            break;

        for (int j = 0; j < 4; j++) // fill with more binary
            binaryAddress[binaryIndex + j] = hexToBinary[j];
                
        binaryIndex += 4; // leap 4 ahead
        // printf("after adding %c -> %s:\n%s\n",hex, hexToBinary, binaryAddress);
        lineIndex++;
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
    int numSetBits = 0;
    int numLines = 0;
    int numBlocks = 0;
    int numBlockBits = 0;
    
    // use getopt to easily extract arguments
    while((opt = getopt(argc, argv, "s:E:b:t:")) != -1) 
    { 

        switch(opt) 
        { 
            case 's':
                numSetBits = atoi(optarg);
                numSets = pow(2, numSetBits);
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

    Cache cache = {
        numSets, 
        (NUM_BITS_IN_ADDRESS - numSetBits - numBlockBits),
        numSetBits, 
        numBlockBits, 
        sets
    };
    // printf("%d\n", cache.numTagBits);
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

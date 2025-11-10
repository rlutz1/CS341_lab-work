#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>  
#include <stdio.h>
#include <string.h>

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
Cache *initCache(int argc, char *argv[]);
void simulate(Cache cache);
void lookForData(Cache cache, int setIndex, char *tag);
void hit(Line line, Line *allLines, int numLines, int rootIndex);
void miss(Set *set, char *tag);
void maxHeapify(Line *A, int parent, int end);
char *convertHexToBinary(char hex);
void getAddressConversion(char *line, char *binaryAddress);
void tryReadCache(void *addr);
void tryWriteCache(void *addr);
void checkNullPtr(void *ptr);
void freeAllMemory(Cache *cache);
void printCache(Cache cache);

int main(int argc, char *argv[])
{    // malloc you fucking idiot
    // (1) extract all arguments and values 
    Cache *cache = initCache(argc, argv);
    // printCache(*cache);

    // (2) go through lines and attempt to interact through cache
    // -- if L: tryReadCache(addr)
    // -- if S: tryWriteCache(addr) // kinda the same as L really
    // -- if M: tryReadCache(addr), tryWriteCache(addr) // load then store
    // ** COUNT HITS AND MISSES AND EVICTIONS!
    simulate(*cache);

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
                printf("%s\n", line);
                
                // we need to (1) snag the argument
                char action = line[ACTION_INDEX];
                // printf("%c\n", action);
                // (2) grab the hex address and convert to binary
                char binaryAddress[NUM_BITS_IN_ADDRESS] = {0};
                getAddressConversion(line, binaryAddress);
                // printf("%s\n", binaryAddress);
                // we can ignore the size here

                // okay, so we have the binary rep now in binaryAddress
                // we need the cache to know how many bits for tag, set, and block offset
                
                // char *tag = (char *) malloc(cache.numTagBits * sizeof(char)); // + 1?
                // checkNullPtr(tag);
                char tag[cache.numTagBits];
                // grab the tag from the binary address
                for (int i = 0; i < cache.numTagBits; i++) {
                    tag[i] = binaryAddress[i];
                } // end loop
                tag[cache.numTagBits] = '\0'; // enforce null for ease of printing
                // printf("kdnsds: %s\n", tag);
                // grab the set bits
                // TODO: get rid of this char[] and just compute the decimal in 1 fell swoop?
                char setNum[cache.numSetBits];
                for (int i = cache.numTagBits, j = 0; j < cache.numSetBits; i++, j++) {
                    setNum[j] = binaryAddress[i];
                } // end loop
                setNum[cache.numSetBits] = '\0'; // enforce null for ease of printing

                // printf("tag: %s\nsetnum: %s\n", tag, setNum);
                // we can snag set bits and convert that to decimal
                int setIndex = 0;
                for (int i = (cache.numSetBits - 1), j = 0; i > -1; i--, j++) {
                    if (setNum[i] == '1') 
                        setIndex += pow(2, j);
                } // end loop

                // printf("set index: %d\n", setIndex);

                switch (action) {
                    case 'L':
                    case 'S':
                        lookForData(cache, setIndex, tag);
                        break;
                    case 'M':
                        lookForData(cache, setIndex, tag);
                        lookForData(cache, setIndex, tag);
                        break;
                    default:
                        printf("Got action not defined? %c\n", action);
                        break;
                } // end switch case
                // for (int i = 0; i < cache.sets[setIndex].numLines; i++) {
                //     printf("%s\n", cache.sets[setIndex].lines[i].tag);
                // }
                
            } // end if
        } // end loop

        fclose(fp);

    } else {
        printf("Something went wrong: trace file name was not given.\n");
    } // end if
} // end method

// we look at cache.sets[set decimal]
// we look at each line (or technically until priority == -1 due to heaping) for the tag. for now, let's look at all
// if (tag == tag): hit count++; hit() // conduct hit procedure
// if we look through all lines an no dice: missCount++ miss()  // call miss procedure 
void lookForData(Cache cache, int setIndex, char *tag) {
    Set currSet = cache.sets[setIndex];
    // printf("curr set hit\n");
    // printf("sakdnasjdbsak");
    printf("tag we're searching for: %s\n", tag);
    for (int i = 0; i < currSet.numLines; i++) {
        Line currLine = currSet.lines[i]; //+ (i * sizeof(Line));
        printf("looking for data, set %d: %d, %s\n", setIndex, currLine.valid, currLine.tag);
        if (currLine.valid == 1 && strcmp(currLine.tag, tag) == 0)  {
            printf("HIT!\n");
            hits++; hit(currLine, currSet.lines, currSet.numLines, i);
            // free(tag);
            return;
        }
    }
    // made it this far, must have been a miss
    printf("MISS!\n");
    misses++; miss(&currSet, tag);

}

void hit(Line currLine, Line *allLines, int numLines, int rootIndex) {
    if ((currLine.priority) != 1) { // if == 1, do nothing
        int oldPriority = currLine.priority;
        (currLine.priority) = 1; // change priority of line to 1
        for (int i = 0; i < numLines; i++) { // increment all priorities < the old one
            Line otherLine = allLines[i];
            if (i != rootIndex && (otherLine.priority) < oldPriority) {
                otherLine.priority++;
            } // end if
        } // end loop
        maxHeapify(allLines, rootIndex, numLines); // root index
    }
}

void maxHeapify(Line *A, int parent, int end) {
    int max = parent;
    int lChild = (2 * parent) + 1;
    int rChild = (2 * parent) + 2;

    // Line *parentPointer = A + (parent * sizeof(Line));
    // Line *lChildPointer = A + (lChild * sizeof(Line));
    // Line *rChildPointer = A + (rChild * sizeof(Line));

    Line parentLine = A[parent];
    Line lChildLine = A[lChild];
    Line rChildLine = A[rChild];

    if (lChild < end && (lChildLine.priority) > (parentLine.priority))
        max = lChild;
    
    if (rChild < end && (rChildLine.priority) > (parentLine.priority))
        max = rChild;
    
    if (max != parent) {
        // Line *temp = A + (max * sizeof(Line));
        // A + (parent * sizeof(Line)) = A + (max * sizeof(Line));
        // A + (max * sizeof(Line)) = parentPointer;
        Line temp = A[parent];
        A[parent] = A[max];
        A[max] = temp;
        maxHeapify(A, max, end);
    }
}

// char *tag; // not sure if this is most appropriate yet; could be better with nuuummbberrr?? since tag is unique, decimal number from it should be as well?
// char valid; // 0 or 1
// short priority; // for mimicking LRU functionality. likely never more than short
// short numBlocks;
// char *data;

void miss(Set *set, char *tag) {
    Line *allLines = (*set).lines;
    Line currLine;
    // int numBlocks = allLines[0].numBlocks;
    for (int i = 0; i < (*set).numLines; i++) {
        currLine = allLines[i];// grab the line
        if (currLine.priority == -1) { // if priority == -1, that means we have an empty slot; we are not concerned with invalidation i guess
            printf("priority -1 is catching \n");
            // increment all other priorities
            for (int j = 0; j < (*set).numLines; j++) {
                if ((*set).lines[j].priority > 0) {
                    (*set).lines[j].priority++;
                } // end if
            } // end loop
            
            // update the first empty line found is now saved with this tag in cache
            strcpy(currLine.tag, tag);
            printf("%s\n", currLine.tag);
            (*set).lines[i].priority = 1;
            (*set).lines[i].valid = 1;

            printf("priority and valid after change: %d, %d\n", (*set).lines[i].priority, (*set).lines[i].valid);
            // grabbing data implied here, but not actually in this simulation
            
            return; // nothing else to do, no eviction, exit the function
        } // end if
    } // end loop

    printf("eviction needed\n");

    // if we manage to get out of this loop, that means we need to evict.
    evictions++;// first, remember to increment eviction count

    // Line root = ; 
    free(allLines[0].tag); // free old tag memory
    strcpy(allLines[0].tag, tag); // insert the new tag/data/priority 1 etc as root of heap
    allLines[0].priority = 1;
    allLines[0].valid = 1; // this shouldn't change, but keeping here for now
    // change of data implied

    // increment all lines after priority
    for (int j = 1; j < (*set).numLines; j++) {
        allLines[j].priority++;
    } // end loop
    
    // fix the heaping
    maxHeapify(allLines, 0, (*set).numLines); // root index

    // free(tag);
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
Cache *initCache(int argc, char *argv[]) {
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
            char *tag = (char *) malloc((NUM_BITS_IN_ADDRESS - numSetBits - numBlockBits) * sizeof(char));
            checkNullPtr(tag);
            Line line = {tag, 0, -1, numBlocks, blocks}; // maybe fine?
            lines[j] = line;
          //  printf("set %d line %d", i, j);
        } // end loop

        Set set = {numLines, lines};
        sets[i] = set;
    } // end loop

    Cache *cache = (Cache *) malloc(sizeof(Cache));
    (*cache).numSets = numSets;
    (*cache).numTagBits = (NUM_BITS_IN_ADDRESS - numSetBits - numBlockBits);
    (*cache).numSetBits = numSetBits;
    (*cache).numBlockBits = numBlockBits;
    (*cache).sets = sets;
    // cache = {
    //     numSets, 
    //     (NUM_BITS_IN_ADDRESS - numSetBits - numBlockBits),
    //     numSetBits, 
    //     numBlockBits, 
    //     sets
    // };
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
void freeAllMemory(Cache *cache) {
    printf("Freeing memory... \n"); 

    for (int i = 0; i < (*cache).numSets; i++) {

        Set set = (*cache).sets[i];
        Line *lines = set.lines;
        for (int j = 0; j < set.numLines; j++) {
            // Line line = set.lines[j];
            Line line = lines[j];
            free(line.data);
            free(line.tag);
        } // end loop
        free(lines);
    }
    free((*cache).sets);
    free(cache);
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

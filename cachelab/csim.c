/**
 * NAME: Roxanne Lutz
 * UNM ID: 101990885
 * 
 * below is the full simulation of an LRU cache
 * true to the design document submitted.
 */

#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>  
#include <stdio.h>
#include <string.h>

// some useful constants
#define MAX_LINE_SIZE 256
#define NUM_HEX_IN_ADDRESS 16
#define NUM_BITS_IN_ADDRESS 64
#define SPACE_INDEX 0 
#define ACTION_INDEX 1
#define ADDRESS_START_INDEX 3
#define NUM_BITS_TO_HEX 4

// struct definitions for ease of reading
typedef struct  {
    char *tag; 
    char valid; 
    short priority; // for mimicking LRU functionality
    short numBlocks;
    char *data;
} Line;

typedef struct {
    int numLines; 
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
static char VERBOSE = 0;
static char *traceFilename = 0;
static int hits = 0;
static int misses = 0;
static int evictions = 0;
static Cache * cache;

// method declarations
Cache *initCache(int argc, char *argv[]);
void simulate();
void lookForData(int setIndex, char *tag, char *fileLine);
void hit(Line *allLines, int numLines, int lineUpdateIndex, char *fileLine);
void miss(Line *allLines, short numLines, char *tag, char *fileLine);
void maxHeapify(Line *A, int parent, int end);
char *convertHexToBinary(char hex);
void getAddressConversion(char *line, char *binaryAddress);
void checkNullPtr(void *ptr);
void *getMemory(short howMany, short size);
void freeAllMemory();

/**
 * @author Roxanne Lutz
 * main method entry point.
 */
int main(int argc, char *argv[]) {     
    cache = initCache(argc, argv); // extract all arguments and values, init the cache 
    simulate();  // go through lines and simulate cache
    printSummary(hits, misses, evictions); // final print summary
    freeAllMemory(); // free all memory allocated for simulation
    return 0;
} // end main method

/**
 * @author Roxanne Lutz
 * main simulation run code. iterate through the file, grab addresses,
 * retrieve needed information, and spur the search through the cache.
 */
void simulate() {
    if (traceFilename) { // if we got a file name from command line
        FILE *fp;
        char line[MAX_LINE_SIZE];

        fp = fopen(traceFilename, "r"); // open file to read

        if (!fp) { // don't do anything if file open failed
            printf("Something went wrong: file open failed.\n"); return;
        } // end if

        short numSetBits = (*cache).numSetBits; // grab the number of set bits
        short numTagBits = (*cache).numTagBits; // grab num tag bits
        short setIndexStart = numTagBits + numSetBits - 1; // where the set info starts in binary address
        char action; // action from the line
        short setIndex; // accumulator counter for set index
        char tag[numTagBits + 1]; // tag builder
        char binaryAddress[NUM_BITS_IN_ADDRESS] = {0}; // hex -> binary builder

        while (fgets(line, sizeof(line), fp)) { // iterate through file

            if (line[SPACE_INDEX] == ' ') { // ignore all I instructions
                action = line[ACTION_INDEX]; // snag the argument
                
                // binaryAddress = {0}; // grab the hex address and convert to binary
                getAddressConversion(line, binaryAddress);

                // build the tag from the address
                for (int i = 0; i < numTagBits; i++) {
                    tag[i] = binaryAddress[i];
                } // end loop
                tag[numTagBits] = '\0'; // enforce null
                
                // gather the set number 
                setIndex = 0;
                for (short i = setIndexStart, j = 0; i >= numTagBits; i--, j++) {
                    if (binaryAddress[i] == '1')
                        setIndex += pow(2, j);
                } // end loop

                switch (action) { // L and S are same, M is just call twice
                    case 'L':
                    case 'S': lookForData(setIndex, tag, line); break;
                    case 'M': lookForData(setIndex, tag, line); lookForData(setIndex, tag, line); break;
                    default:  printf("Got action not defined? %c\n", action); break;
                } // end switch case
            } // end if
        } // end loop

        fclose(fp); // close file
    } else {
        printf("Something went wrong: trace file name was not given.\n");
    } // end if
} // end method

/**
 * @author Roxanne Lutz
 * just a simple method to use to bundle up the search for a hit.
 * and handle a miss if occurrs.
 */
void lookForData(int setIndex, char *tag, char *fileLine) {
    Set currSet = (*cache).sets[setIndex];
    Line currLine;

    for (int i = 0; i < currSet.numLines; i++) { // iterate through the lines
        currLine = currSet.lines[i];
        // if a valid line and tags are the same, huzzah, a hit!
        if (currLine.valid == 1 && strcmp(currLine.tag, tag) == 0)  {
            hits++; hit(currSet.lines, currSet.numLines, i, fileLine); return;
        }
    } // end loop

    // if we made it this far, it must be a miss
    misses++; miss(currSet.lines, currSet.numLines, tag, fileLine);
} // end method

/**
 * @author Roxanne Lutz
 * hit procedure. 
 * if the hit line is not already priority one:
 * change it to one,
 * increment all other priorities on non-empty lines by one that have priority less than the old one,
 * fix the heap from the hit line down.
 */
void hit(Line *allLines, int numLines, int lineUpdateIndex, char *fileLine) {
    if (VERBOSE) // printing
        printf("%s -- hit \n", fileLine);

    short oldPriority = allLines[lineUpdateIndex].priority; // grab the old priority of hit line
    if (oldPriority != 1) { // if == 1, do nothing
        allLines[lineUpdateIndex].priority = 1; // change priority of line to 1
        short otherPriority;

        for (int i = 0; i < numLines; i++) { // increment all non-empty line priorities < the old one
            otherPriority = allLines[i].priority;
            if (i != lineUpdateIndex && otherPriority > 0 && otherPriority < oldPriority) 
                allLines[i].priority++;
        } // end loop
        maxHeapify(allLines, lineUpdateIndex, numLines); // max heap fix from this changed hit line
    } // end if
} // end method

/**
 * @author Roxanne Lutz
 * miss procedure. 
 * first, check to see if there is an empty slot, which will always be
 * leaves of heap here. we do not care about invalidated data either.
 * if there is no space, we perform an eviction of LRU, fixing the heap
 */
void miss(Line *allLines, short numLines, char *tag, char *fileLine) {
    if (VERBOSE) // printing
        printf("%s -- miss\n", fileLine);

    for (int i = 0; i < numLines; i++) { // check through all lines
        if (allLines[i].priority == -1) { // if priority == -1, that means we have an empty slot; we are not concerned with invalidation 
            for (int j = 0; j < numLines; j++) {  // increment all other priorities
                if (allLines[j].priority > 0) // don't increment empties
                    allLines[j].priority++;
            } // end loop
            
            // update the first empty line found is now saved with this tag in cache
            strcpy(allLines[i].tag, tag); // copy in this new tag
            allLines[i].priority = 1; // new priority 1 for this guy
            allLines[i].valid = 1; // ensuring it stays valid, but this shouldn't change in sim

            return; // nothing else to do, no eviction, exit the function
        } // end if
    } // end loop

    if (VERBOSE) // printing
        printf("%s  -- eviction\n", fileLine);

    // if we manage to get out of this loop, that means we need to evict.
    evictions++;// first, remember to increment eviction count

    // insert the new tag/data/priority 1 etc as root of heap
    strcpy(allLines[0].tag, tag); // copy in this new tag
    allLines[0].priority = 1; // new priority 1 for this guy
    allLines[0].valid = 1; // ensuring it stays valid, but this shouldn't change in sim

    for (int j = 1; j < numLines; j++) // increment all lines after priority
        allLines[j].priority++;
    
    maxHeapify(allLines, 0, numLines); // max heapify from root (sift the priority 1 down)
} // end method

/**
 * @author Roxanne Lutz
 * basic max heapify implementation. 
 * simply maintains the heap of the lines as needed
 */
void maxHeapify(Line *A, int parent, int end) {
    int max = parent; // assume max is parent
    int lChild = (2 * parent) + 1; // left child
    int rChild = (2 * parent) + 2; // right child

    // get max of all (parent, left, right)
    if (lChild < end && (A[lChild].priority) > (A[max].priority))
        max = lChild;
    
    if (rChild < end && (A[rChild].priority) > (A[max].priority))
        max = rChild;
    
    if (max != parent) { // if swap needed, swap
        Line temp = A[parent];
        A[parent] = A[max];
        A[max] = temp;
        maxHeapify(A, max, end); // recur to subtree
    } // end if
} // end method

/**
 * @author Roxanne Lutz
 * purpose of this method is to grab the hex address and convert to 
 * binary. return it as a string of binary numbers.
 * number of bits is at least 64
 */
void getAddressConversion(char *fileLine, char *binAddressContainer) {
    char hex; // holder for the hex char
    char *hexToBinary; // holder for hex to binary conversion
    char binaryIndex = 0; // index for filling binary rep of address
    char countHex = 0; // for counting how many hex given
    short lineIndex = ADDRESS_START_INDEX; // start of the address in line

    // count how many hex chars there are in given address
    while (fileLine[lineIndex] != ',') {
        countHex++;
        lineIndex++;
    } // end loop

    lineIndex = ADDRESS_START_INDEX; // reset

    // calculate how many filler zeros needed at start of binary address
    char fillerZeros = (NUM_HEX_IN_ADDRESS - countHex) * NUM_BITS_TO_HEX; 
    for (short i = 0; i < fillerZeros; i++) // fill with filler zeros
        binAddressContainer[i] = '0';

    binaryIndex = fillerZeros; // offset the index by num filler zeros

    while ((hex = fileLine[lineIndex]) != ',') { 
        hexToBinary = convertHexToBinary(hex); // get the binary of this hex char

        for (int j = 0; j < NUM_BITS_TO_HEX; j++) // fill with more binary
            binAddressContainer[binaryIndex + j] = hexToBinary[j];
                
        binaryIndex += NUM_BITS_TO_HEX; // leap 4 ahead
        lineIndex++; // move next character in line
    } // end loop
} // end method

/**
 * @author Roxanne Lutz
 * parse and then initialize the cache based on arguments passed.
 */
Cache *initCache(int argc, char *argv[]) {
    int opt;
    int numSets = 0;
    int numSetBits = 0;
    int numLines = 0;
    int numBlocks = 0;
    int numBlockBits = 0;
    
    // use getopt to easily extract arguments
    while((opt = getopt(argc, argv, "s:E:b:t:v")) != -1) 
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
            case 'v':
                VERBOSE = 1;
                break;
            case ':': printf("option needs a value\n"); break; 
            case '?': printf("unknown option: %c\n", optopt); break; 
        } // end switch case
    }  // end loop

    Set *sets = (Set *) getMemory(numSets, sizeof(Set)); // init the set
    short numTagBits = NUM_BITS_IN_ADDRESS - numSetBits - numBlockBits; // calculate the number of tag bits needed
    
    for (int i = 0; i < numSets; i++) {
        Line *lines = (Line *) getMemory(numLines, sizeof(Line)); // create a line pointer for all lines in set

        for (int j = 0; j < numLines; j++) {
            char *tag = (char *) getMemory(numTagBits + 1, sizeof(char)); // create a tag for every line

            Line line = {tag, 0, -1, numBlocks, 0}; // init with: tag, valid = 0, priority = -1, numblocks, and null block ptr for data space
            lines[j] = line; // and line to appropriate slot
        } // end loop

        Set set = {numLines, lines}; // initialize this set
        sets[i] = set; // add to the set list
    } // end loop

    Cache *cache = (Cache *) getMemory(1, sizeof(Cache)); // initialize the parent cache ptr and memory

    // initialize all fields
    (*cache).numSets = numSets;
    (*cache).numTagBits = numTagBits;
    (*cache).numSetBits = numSetBits;
    (*cache).numBlockBits = numBlockBits;
    (*cache).sets = sets;

    return cache; // return parent cache ptr
} // end method

/**
 * @author Roxanne Lutz
 * VERY quick and dirty hex conversion method. 
 * this is hardcoded for ease of use.
 */
char *convertHexToBinary(char hex) {
    switch (hex) { 
            case '0': return "0000"; break;
            case '1': return "0001"; break;
            case '2': return "0010"; break;
            case '3': return "0011"; break;
            case '4': return "0100"; break;
            case '5': return "0101"; break;
            case '6': return "0110"; break;
            case '7': return "0111"; break;
            case '8': return "1000"; break;
            case '9': return "1001"; break;
            case 'A':
            case 'a': return "1010"; break;
            case 'B':
            case 'b': return "1011"; break;
            case 'C':
            case 'c': return "1100"; break;
            case 'D':
            case 'd': return "1101"; break;
            case 'E':
            case 'e': return "1110"; break;
            case 'F':
            case 'f': return "1111"; break;
        } // end switch case;
        // exit gracefully if we got something strange
        printf("Something went wrong: received an undefined hex value: %c", hex);
        freeAllMemory();
        exit(2);
} // end method

/**
 * @author Roxanne Lutz
 * simple utility method to check for nulls and 
 * free upon exit. 
 */
void checkNullPtr(void *ptr) {
    if (!ptr) {
        printf("Null pointer returned by malloc. Exiting.\n");
        freeAllMemory();
        exit(1);
    } // end if
} // end method

/**
 * @author Roxanne Lutz
 * method to malloc a ptr dynamically, check for null.
 * just makes my life less complicated.
 */
void *getMemory(short howMany, short size) {
    void *ptr = malloc(howMany * size);
    checkNullPtr(ptr);
    return ptr;   
} // end method

/**
 * @author Roxanne Lutz
 * need to ensure freeing of all malloc'd memory upon program exit.
 * malloc is really only used for cache, so this just goes through the 
 * cache and frees any non-null pointers made in intialization. 
 * this could get called at strange times, so we need to check for null
 * as we go.
 */
void freeAllMemory() {
    // printf("Freeing memory... \n"); 
    if (cache) {
        short numSets = (*cache).numSets;
        short numLines;
        Line *lines;
        char *tag;
        Set *sets = (*cache).sets;
        if (sets) {
            for (short i = 0; i < numSets; i++) {
                    if (lines) {
                        lines = (*cache).sets[i].lines; // get line ptr
                        numLines = (*cache).sets[i].numLines; // number of lines in set
                        for (short j = 0; j < numLines; j++) {
                            tag = lines[j].tag;
                            if (tag) 
                                free(tag); // free all malloc tags 
                        } // end loop
                        free(lines); // free the set's line ptr
                    } // end if
            } // end loop
            free(sets); // free the set ptr
        } // end if
        free(cache); // free the cache ptr
    } // end if
} // end method

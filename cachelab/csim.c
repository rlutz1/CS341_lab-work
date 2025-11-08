#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>  
#include <stdio.h>

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
void parseFile();
void tryReadCache(void *addr);
void tryWriteCach(void *addr);
void checkNullPtr(void *ptr);
void freeAllMemory(Cache cache);
void printCache(Cache cache);

int main(int argc, char *argv[])
{    // malloc you fucking idiot
    // (1) extract all arguments and values 
    Cache cache = initCache(argc, argv);
    printCache(cache);

    // (2) parse the file given into iterable lines. only needed data. 
    // -- parseFile()
    // (2) go through lines and attempt to interact through cache
    // -- if L: tryReadCache(addr)
    // -- if S: tryWriteCache(addr) // kinda the same as L really
    // -- if M: tryReadCache(addr), tryWriteCache(addr) // load then store
    // ** COUNT HITS AND MISSES AND EVICTIONS!
    printSummary(hits, misses, evictions);
    freeAllMemory(cache);
    return 0;
}

void simluate() {
    if (traceFilename) {
        FILE *fp;
        char *line = 0;
        size_t len = 0;
        ssize_t read;

        fp = fopen(traceFilename, "r");
        
        if (!fp) {
            printf("Something went wrong: file open failed.\n"); return;
        } // end if

        while ((read = getline(&line, &len, fp)) != -1) {
            printf("Retrieved line of length %zu:\n", read);
            printf("%s", line);
        } // end loop
        fclose(fp);

        if (line)
            free(line);

    } else {
        printf("Something went wrong: trace file name was not given.\n");
    } // end if
} // end method

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

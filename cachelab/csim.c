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

typedef struct {
    int numSets;
    int numSetIndexBits;
    int numLines;
    int numBlocks;
    int numBlockBits;
    char *traceFilename;
} SetupInfo;

// globals
int hits = 0;
int misses = 0;
int evictions = 0;

// method declarations
Cache extractArgs(int argc, char *argv[]);
void parseFile();
void tryReadCache(void *addr);
void tryWriteCach(void *addr);

int main(int argc, char *argv[])
{
    // (1) extract all arguments and values 
    Cache cache = extractArgs(argc, argv);
    
    printf("number of sets: %d \n", cache.numSets); 

    for (int i = 0; i < cache.numSets; i++) {

        Set set = cache.sets[i];
        printf("num lines in set %d: %d \n", i, set.numLines);
        for (int j = 0; j < set.numLines; j++) {
            Line line = set.lines[j];
    // char *tag; // not sure if this is most appropriate yet; could be better with nuuummbberrr?? since tag is unique, decimal number from it should be as well?
    // char valid; // 0 or 1
    // short priority; // for mimicking LRU functionality. likely never more than short
    // short numBlocks;
    // char *data;
            printf("LINE %D \n", j);
            printf("tag in line: %s \n", line.tag);
            printf("valid bit: %d \n", line.valid);
            printf("priority: %d \n", line.priority);
            printf("number of bytes of data in line: %d \n", line.numBlocks);
            printf("data in blocks of line: %s \n", line.data);
            printf("END LINE\n");
        }
    }
    // printf("%d",cache.numSetIndexBits); printf("\n");
    // printf("%d",cache.numLines); printf("\n"); // E
    // printf("%d",cache.numBlocks); printf("\n"); // b
    // printf("%d",cache.numBlockBits); printf("\n");
    // printf("%s",cache.traceFilename); printf("\n");
    // (2) parse the file given into iterable lines. only needed data. 
    // -- parseFile()
    // (3) go through lines and attempt to interact through cache
    // -- if L: tryReadCache(addr)
    // -- if S: tryWriteCache(addr) // kinda the same as L really
    // -- if M: tryReadCache(addr), tryWriteCache(addr) // load then store
    // ** COUNT HITS AND MISSES AND EVICTIONS!
    printSummary(hits, misses, evictions);
    return 0;
}

Cache extractArgs(int argc, char *argv[]) {
    int opt;
    SetupInfo si = {0, 0, 0, 0};
   
    // put ':' in the starting of the
    // string so that program can 
    //distinguish between '?' and ':' 
    while((opt = getopt(argc, argv, "s:E:b:t:")) != -1) 
    { 
        switch(opt) 
        { 
            case 's':
                si.numSetIndexBits = atoi(optarg);
                si.numSets = pow(2, si.numSetIndexBits);
                break;
            case 'E': 
                si.numLines = atoi(optarg);
                break;
            case 'b':
                si.numBlockBits = atoi(optarg);
                si.numBlocks = pow(2, si.numBlockBits);
                break;
            case 't':
                si.traceFilename = optarg;
                break;
            case ':': 
                printf("option needs a value\n"); 
                break; 
            case '?': 
                printf("unknown option: %c\n", optopt);
                break; 
        } // end switch case
    }  // end loop


// struct definitions for ease of reading
// typedef struct  {
//     char *tag; // not sure if this is most appropriate yet; could be better with nuuummbberrr?? since tag is unique, decimal number from it should be as well?
//     char valid; // 0 or 1
//     short priority; // for mimicking LRU functionality. likely never more than short
//     char data[];
// } Line;

// typedef struct {
//     int numLines; // added because C is complaining with it, boo
//     Line lines[];
// } Set;


    Set sets[si.numSets];
    for (int i = 0; i < si.numSets; i++) {
        
        Line lines[si.numLines];

        for (int j = 0; j < si.numLines; j++) {
            char blocks[si.numBlocks];
            Line line = {0, 0, -1, si.numBlocks, blocks};
            lines[j] = line;
        } // end loop

        Set set = {si.numLines, lines};
        sets[i] = set;
    } // end loop

    Cache cache = {si.numSets, sets};
    // return si;
    return cache;
    // TODO: test this and print out the shtuff.
}

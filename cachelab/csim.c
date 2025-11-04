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
    char data[];
} Line;

typedef struct {
    int numLines; // added because C is complaining with it, boo
    Line lines[];
} Set;

typedef struct {
    int numSets;
    Set sets[];
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
SetupInfo extractArgs(int argc, char *argv[]);
void parseFile();
void tryReadCache(void *addr);
void tryWriteCach(void *addr);

int main(int argc, char *argv[])
{
    // (1) extract all arguments and values 
    SetupInfo si = extractArgs(argc, argv);
    printf("%d", si.numSets); printf("\n"); // s
    printf("%d",si.numSetIndexBits); printf("\n");
    printf("%d",si.numLines); printf("\n"); // E
    printf("%d",si.numBlocks); printf("\n"); // b
    printf("%d",si.numBlockBits); printf("\n");
    printf("%s",si.traceFilename); printf("\n");
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

SetupInfo extractArgs(int argc, char *argv[]) {
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
    return si;
    // TODO: test this and print out the shtuff.
}

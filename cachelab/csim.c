#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

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

// globals
int hits = 0;
int misses = 0;
int evictions = 0;

// method declarations
void extractArgs();
void parseFile();
void tryReadCache(void *addr);
void tryWriteCach(void *addr);

int main()
{
    // (1) extract all arguments and values 
    // -- extractArgs()
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

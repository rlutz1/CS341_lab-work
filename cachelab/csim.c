#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

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

int main()
{
    // (1) extract all arguments and values
    // (2) parse the file given into iterable lines. only needed data.
    // (3) go through lines and attempt to interact through cache
    // -- if L: tryReadCache(addr)
    // -- if S: tryWriteCache(addr) // kinda the same as L really
    // -- if M: tryReadCache(addr), tryWriteCache(addr)
    printSummary(0, 0, 0);
    return 0;
}

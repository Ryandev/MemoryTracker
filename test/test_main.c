
#include <stdio.h>
#include <string.h>

#include "memoryTrackerFacade.h"
#include "test_memManager.h"


int main(int argc, const char * argv[])
{

    memInit;

    bool testpassed = test_memMan();
    
    fprintf(stdout, "MemoryTracker passed:%s\n", (testpassed)?"yes":"no") ;
    
    memTerm;
    
    return (testpassed)?0:1;
}


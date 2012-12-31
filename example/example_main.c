
#include <stdio.h>
#include <string.h>
#include "memoryTrackerFacade.h"


void ex1 ( void )
{
    volatile void * ptr = memAlloc(128);
    ptr = ptr;
}

void ex2 ( void )
{
    void * ptr = memAlloc(128);
    
    if ( ptr )
    {
        memFree(ptr);
    }
}

void ex3 ( void )
{
    volatile void * ptr = memAlloc(256);
    ptr = ptr;
}

int main(int argc, const char * argv[])
{

    memInit;
    
    fprintf(stdout, "\nUnfree'd memory allocations before:\n");
    
    memPrintAllocations;
    
    ex1();
    ex2();
    ex3();

    fprintf(stdout, "\n\nUnfree'd memory allocations after:\n");

    memPrintAllocations;
    
    memTerm;
    
    return 0;
}


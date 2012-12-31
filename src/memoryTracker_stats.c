/**
 @file
 Memory Tracker used to track mallocs - statistics
 
 @author Ryan Powell
 @date 03-10-11
 @copyright
     Copyright (c) 2011  Ryan Powell \n
     Proprietary & Confidential \n
     This file & associated documentation may not be used without the consent of the authors permission. \n
     Undocumented use of this material shall be an infringement of copyright. \n
 */


#include <execinfo.h>

#include "memoryTrackerSettings.h"
#include "memoryTracker_stats.h"
#include "memoryTracker_list.h"


uint32_t memoryTracker_stats_totalBytesAllocated ( void )
{
    uint32_t allocedBytes   = 0U;
    uint32_t index          = 0U;
    
    for ( index=0U; index<memoryTracker_list_count(); index++ )
    {
        MALLOCNODE * node = memoryTracker_list_getAtIndex(index);
        
        MEMTRK_ASSERT(node!=NULL);
        
        if ( node != NULL )
        {
            allocedBytes += node->allocSize; 
        }
    }
    
    return allocedBytes;    
}

uint32_t memoryTracker_stats_totalOutstandingAllocs ( void )
{
    return memoryTracker_list_count();
}

void memoryTracker_stats_printDebugForNode ( MALLOCNODE * node, char * reason, FILE * file )
{
    if ( node )
    {
        /* print the cause for printing */
        if ( reason )
        {
            fprintf(file, "%s\n",reason);
        }
        
        fprintf(file, "F:%s Fn:%s L:%d Sz:%lu | Trace:\n",(char*)node->fileName,(char*)node->functionName,node->lineNumber,node->allocSize);
        
        /* print the stack trace symbols */
#if MEMORYTRACKER_PRINT_STACKTRACE_AS_STRINGS == 1
        char ** strings = backtrace_symbols (node->stacktrace, node->stackSize);
        
        for ( uint8_t i = 0U; i<node->stackSize; i++ )
        {
            fprintf(file, "%s",strings[i]);
            
            if ( i != node->stackSize - 1 )
            {
                fprintf(file," --> ");
            }
            
            fprintf(file, "\n");
        }
#else
        for ( uint8_t i = 0U; i<node->stackSize; i++ )
        {
            fprintf(file, "%p",node->stacktrace[i]);
            
            if ( i != node->stackSize - 1 )
            {
                fprintf(file," --> ");
            }
        }
        
        fprintf(file, "\n");
#endif
        fprintf(file,"\n");
    }
    else
    {
        fprintf(file, " --- Invalid node --- \n");
    }
}

void memoryTracker_stats_printDebug ( FILE * file )
{
    uint32_t index = 0U;
    
    fprintf(file, "\n  ------   MEMORY ALLOCATIONS   ------  \n\n");
    
    for ( index=0U; index<memoryTracker_stats_totalOutstandingAllocs(); index++ )
    {
        MALLOCNODE * node = memoryTracker_list_getAtIndex(index);
        
        MEMTRK_ASSERT(node!=NULL);
        
        fprintf(file, "%u. ",index);
        
        if ( node )
        {
            memoryTracker_stats_printDebugForNode ( node, NULL, file );
        }
        else
        {
            fprintf(file, " --- Invalid node at %u --- \n",index);
        }
    }
    
    fprintf(file, "\n  ------   MEMORY ALLOCATIONS   ------  \n\n");
}

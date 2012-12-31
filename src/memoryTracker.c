/**
 @file
 Memory Tracker used to track mallocs
 
 @author Ryan Powell
 @date 03-10-11
 @copyright
     Copyright (c) 2011  Ryan Powell \n
     Proprietary & Confidential \n
     This file & associated documentation may not be used without the consent of the authors permission. \n
     Undocumented use of this material shall be an infringement of copyright. \n
 */


#include <stdio.h>                  /* for malloc */
#include <string.h>                 /* for memcpy */

#include "memoryTracker.h"
#include "memoryTracker_common.h"
#include "memoryTracker_node.h"
#include "memoryTracker_list.h"
#include "memoryTracker_stats.h"


void memoryTracker_init ( void )
{
    memoryTracker_list_init();
}

void memoryTracker_term ( void )
{
    memoryTracker_list_term();
}

void * memoryTracker_alloc ( const char *sourceFile, const unsigned int sourceLine, const char *sourceFunc, const size_t reqSize )
{
    void * retPtr = NULL;

    if ( reqSize != 0 )
    {
        MALLOCNODE * node = memoryTracker_node_create(sourceFile,sourceLine,sourceFunc,reqSize);
        
        if ( node )
        {
            if ( memoryTracker_list_insert(node) )
            {
                MEMTRK_LOG_I("Alloc: {%p} %s:%d COUNT:%d!",node->mallocPointer, sourceFile,sourceLine,memoryTracker_allocatedCount());
                retPtr = node->mallocPointer;
            }
            else
            {
                memoryTracker_node_remove(node);
                MEMTRK_LOG_E("Alloc: FAILED. could not insert node");                    
            }
        }
        else
        {
            MEMTRK_LOG_E("Failed to create node structure");
        }
    }

    return retPtr;
}

void * memoryTracker_realloc ( const char *sourceFile, const unsigned int sourceLine, const char *sourceFunc, void * ptr, const size_t reqSize )
{
    void * retPtr = NULL;
    
    if ( ( reqSize != 0 ) && ( ptr != NULL ) && ( reqSize != 0 ) )
    {
        MALLOCNODE * newNode = memoryTracker_node_create(sourceFile,sourceLine,sourceFunc,reqSize);
        MALLOCNODE * oldNode = memoryTracker_node_getNodeFromPointer(ptr);
        
        if ( ( oldNode ) && ( newNode ) )
        {
            MEMTRK_ASSERT(oldNode->allocSize!=newNode->allocSize);
            
            if ( memoryTracker_list_insert(newNode) )
            {                
                MEMTRK_ASSERT(oldNode->allocSize>=reqSize);

                memcpy(newNode->mallocPointer,oldNode->mallocPointer,oldNode->allocSize);
                retPtr = newNode->mallocPointer;
                
                MEMTRK_LOG_I("Realloced ptr:%p->%p resized:%lu->%lu",ptr,retPtr,oldNode->allocSize,reqSize);

                memoryTracker_list_remove(oldNode);
            }
            else
            {
                memoryTracker_node_remove(newNode);
                MEMTRK_LOG_E("Alloc: FAILED. could not insert node");                    
            }
        }
        else
        {
            MEMTRK_LOG_E("Failed to create node structure");
        }
    }
    
    return retPtr;
}

void memoryTracker_free ( const char *sourceFile, const unsigned int sourceLine, const char *sourceFunc, void * memLocation )
{
    if ( memLocation != NULL )
    {
        MALLOCNODE * node = memoryTracker_node_getNodeFromPointer(memLocation);

        if ( node )
        {
            /* remove it from tracking */
            memoryTracker_list_remove(node);

            /* release the associated memory */
            memoryTracker_node_remove(node);

            MEMTRK_LOG_I("Free:  {%p} %s:%d COUNT:%d!",memLocation,sourceFile,sourceLine,memoryTracker_allocatedCount());
        }
        else
        {
            MEMTRK_LOG_E("Could not find NODE: %p",memLocation);
        }
    }
}

bool memoryTracker_isPointerValid ( void * mem_ptr )
{
    bool isValid = true;

    if ( mem_ptr )
    {
        MALLOCNODE * node = memoryTracker_node_getNodeFromPointer(mem_ptr);
        isValid = memoryTracker_node_isNodeValid(node);
    }

    return isValid;
}

uint32_t memoryTracker_allocatedBytes ( void )
{
    return memoryTracker_stats_totalBytesAllocated();
}

uint32_t memoryTracker_allocatedCount ( void )
{
    return memoryTracker_stats_totalOutstandingAllocs();
}

void printAllocations ( void )
{
    memoryTracker_stats_printDebug(stdout);
}

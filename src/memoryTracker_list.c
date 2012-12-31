/**
 @file
 Memory Tracker used to track mallocs - #_MALLOCNODE entry management
 
 @author Ryan Powell
 @date 03-10-11
 @copyright
     Copyright (c) 2011  Ryan Powell \n
     Proprietary & Confidential \n
     This file & associated documentation may not be used without the consent of the authors permission. \n
     Undocumented use of this material shall be an infringement of copyright. \n
 */


#include <string.h>     /* for memset */
#include <stdlib.h>

#include "rbtree.h"
#include "memoryTracker_list.h"


static RBTREE_HANDLE f_handle = RBTREE_HANDLE_INVALID;


bool memoryTracker_list_init ( void )
{
    bool success = false;
    
    if ( rbtree_createTree(&f_handle, malloc, free) == RBTREE_STATUS_OK )
    {
        success = true;
    }
    
    return success;
}

bool memoryTracker_list_term ( void )
{
    bool success = false;
    
    if ( rbtree_destroyTree(f_handle) == RBTREE_STATUS_OK )
    {
        success = true;
    }
    
    return success;
}

bool memoryTracker_list_insert ( MALLOCNODE * node )
{
    bool status = false;

    MEMTRK_ASSERT(f_handle!=RBTREE_HANDLE_INVALID);
    
    if ( rbtree_insert(f_handle, (void*)node, &node->key) == RBTREE_STATUS_OK )
    {
        status = true;
    }

    return status;
}

bool memoryTracker_list_remove ( MALLOCNODE * node )
{
    bool wasMatchFound = false;
    
    MEMTRK_ASSERT(f_handle!=RBTREE_HANDLE_INVALID);

    if ( rbtree_deleteByKey(f_handle, node->key) == RBTREE_STATUS_OK )
    {
        wasMatchFound = true;
    }
    
    return wasMatchFound;
}

bool memoryTracker_list_findMemoryPointer_cmp (void* storevalue, void* userdata)
{
    bool isMatch = false;
    
    MEMTRK_ASSERT(f_handle!=RBTREE_HANDLE_INVALID);
    
    if ( storevalue )
    {
        MALLOCNODE * node = (MALLOCNODE *)storevalue;
        
        if ( node->mallocPointer == userdata )
        {
            isMatch = true;
        }
    }
    
    return isMatch;
}

uint32_t memoryTracker_list_count ( void )
{
    uint32_t node_count = 0U;
    
    MEMTRK_ASSERT(f_handle!=RBTREE_HANDLE_INVALID);
    
    RBTREE_STATUS status = rbtree_entryCount(f_handle, &node_count);

    status = status; /* remove compiler warning - unused variable */
    MEMTRK_ASSERT(status==RBTREE_STATUS_OK);

    return node_count;
}

MALLOCNODE * memoryTracker_list_getAtIndex ( uint32_t position )
{
    MALLOCNODE * node = NULL;
    RBTREE_KEY key = RBTREE_KEY_INVALID;

    MEMTRK_ASSERT(f_handle!=RBTREE_HANDLE_INVALID);
    
    RBTREE_STATUS status = rbtree_retrieveByIndex(f_handle, position, (void**)&node, &key);
    
    status = status; /* remove compiler warning - unused variable */
    MEMTRK_ASSERT(status==RBTREE_STATUS_OK);
    
    return node;
}

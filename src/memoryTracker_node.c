/**
 @file
 Memory Tracker used to track mallocs - node construction/deletion
 
 @author Ryan Powell
 @date 03-10-11
 @copyright
     Copyright (c) 2011  Ryan Powell \n
     Proprietary & Confidential \n
     This file & associated documentation may not be used without the consent of the authors permission. \n
     Undocumented use of this material shall be an infringement of copyright. \n
 */


#include <string.h>     /* for memset */
#include <stdlib.h>     /* for malloc */
#include <execinfo.h>   /* for backtrace() */

#include "memoryTracker_node.h"
#include "memoryTracker_stats.h"
#include "memoryTrackerSettings.h"


/** pattern to wipe over the #_MALLOCNODE */
#define MEMORYTRACKER_NODE_WIPE_PATTERN 0x33


#ifdef _WIN32
    #define FILESYSTEM_DIRECTORY_SEPERATOR '\'
#else
    #define FILESYSTEM_DIRECTORY_SEPERATOR '/'
#endif


static inline void * memoryTracker_node_prv_alloc ( size_t allocSize )
{
    void * ptr = malloc( allocSize );

    if ( ptr )
    {
        MEMTRK_LOG_E("Alloced %p",ptr);
    }

    return ptr;
}

static inline void memoryTracker_node_prv_free ( void * ptr )
{
    if ( ptr )
    {
        free(ptr);
        MEMTRK_LOG_E("Free'd %p",ptr);
    }
}

static inline void memoryTracker_node_prv_wipeBlock ( void * ptr, size_t allocSize )
{    
    MEMTRK_ASSERT(allocSize>sizeof(MALLOCNODE));
    
    if ( ptr )
    {
        memset(ptr, MEMORYTRACKER_NODE_WIPE_PATTERN, allocSize);

        void * userPointer = ptr + sizeof(MALLOCNODE) + MEMORYTRACKER_BOUNDARY_TAGS_SIZE;
        size_t userSize = allocSize - sizeof(MALLOCNODE) - (2*MEMORYTRACKER_BOUNDARY_TAGS_SIZE);

        int wipeUser = (int)MEMORYTRACKER_USER_WIPE_PATTERN;
        
        /* user alloc wipe */
        memset( userPointer, wipeUser, userSize );

        void * preTagPointer = userPointer - MEMORYTRACKER_BOUNDARY_TAGS_SIZE;
        
        int tagPre = (int)MEMORYTRACKER_BOUNDARY_TAGS_PREINSERT;
        
        /* pre tag wipe */
        memset( preTagPointer, tagPre, MEMORYTRACKER_BOUNDARY_TAGS_SIZE );

        void * postTagPointer = ( ptr + allocSize ) - MEMORYTRACKER_BOUNDARY_TAGS_SIZE;

        int tagPost = (int)MEMORYTRACKER_BOUNDARY_TAGS_POSTINSERT;
        
        /* post tag wipe */
        memset( postTagPointer, tagPost, MEMORYTRACKER_BOUNDARY_TAGS_SIZE );
    }
}

static inline MALLOCNODE * memoryTracker_node_prv_getNodeFromPointer ( void * ptr )
{
    MALLOCNODE * retNode = NULL;
    
    retNode = ptr - MEMORYTRACKER_BOUNDARY_TAGS_SIZE - sizeof(MALLOCNODE);
    
    return retNode;    
}

static inline void * memoryTracker_node_prv_getBoundaryPointerPre ( MALLOCNODE * node )
{
    return (void *) node->mallocPointer - MEMORYTRACKER_BOUNDARY_TAGS_SIZE;
}

static inline void * memoryTracker_node_prv_getBoundaryPointerPost ( MALLOCNODE * node )
{
    return (void *) node->mallocPointer + node->allocSize;
}


static inline bool memoryTracker_node_prv_isBoundaryTagValid ( void * bnd_tag_check, uint8_t tag_check )
{
    bool status = true;
    
    uint8_t * bnd_tag = (uint8_t *) bnd_tag_check;
    
    for ( uint8_t i = 0U; i<MEMORYTRACKER_BOUNDARY_TAGS_SIZE; i++ )
    {
        if ( bnd_tag[i] != tag_check )
        {
            MEMTRK_LOG_E("Boundary tag overwritten %c tag:%c\n",bnd_tag[i],tag_check);
            status = false;
            break;
        }
    }
    
    return status;
}

static void memoryTracker_node_baseName ( char * filename, char * baseFileName, size_t baseFileNameSize )
{
    size_t offsetForLocalPathOnly = 0U;
    size_t index = 0U;
    
    while ( filename[index] != '\0' )
    {
        if ( filename[index] == FILESYSTEM_DIRECTORY_SEPERATOR )
        {
            offsetForLocalPathOnly = index;
        }
        
        index++;
    }
    
    /* +1 to ignore the / prefix */
    snprintf((char *)baseFileName, baseFileNameSize, "%s",(char*)&filename[offsetForLocalPathOnly+1]);
}

bool memoryTracker_node_remove ( MALLOCNODE * node )
{
    bool status = true;

    if ( node )
    {
        void ** pre_tag     = memoryTracker_node_prv_getBoundaryPointerPre(node);
        void ** post_tag    = memoryTracker_node_prv_getBoundaryPointerPost(node);
        
        if ( memoryTracker_node_prv_isBoundaryTagValid( pre_tag, (uint8_t)MEMORYTRACKER_BOUNDARY_TAGS_PREINSERT ) == false )
        {
            char msg[48];
            snprintf(msg,48,"Boundary Tag-post is invalid %p",node->mallocPointer);
            memoryTracker_stats_printDebugForNode(node, msg, stdout);
            status = false;
        }

        if ( memoryTracker_node_prv_isBoundaryTagValid( post_tag, (uint8_t)MEMORYTRACKER_BOUNDARY_TAGS_POSTINSERT ) == false )
        {
            char msg[48];
            snprintf(msg,48,"Boundary Tag-pre is invalid %p",node->mallocPointer);
            memoryTracker_stats_printDebugForNode(node, msg, stdout);
            status = false;
        }
        
        /* this will free node->mallocPointer since they are one */
        memoryTracker_node_prv_free((void*)node);
    }
    else
    {
        MEMTRK_LOG_E("NULL param passed to %s\n",__FUNCTION__);
        status = false;
    }
    
    return status;
}

MALLOCNODE * memoryTracker_node_create ( const char *sourceFile, const unsigned int sourceLine, const char *sourceFunc, const size_t reqSize )
{
    MALLOCNODE * node = NULL;
    
    size_t allocSize = sizeof(MALLOCNODE) + reqSize + ( MEMORYTRACKER_BOUNDARY_TAGS_SIZE * 2 );
    
    void * ptr = memoryTracker_node_prv_alloc(allocSize);

    if ( ptr != NULL )
    {
        memoryTracker_node_prv_wipeBlock(ptr,allocSize);

        void * mallocPointer = ptr + sizeof(MALLOCNODE) + MEMORYTRACKER_BOUNDARY_TAGS_SIZE;

        node = (MALLOCNODE*)ptr;
        
        node->key = RBTREE_KEY_INVALID;
        
        memoryTracker_node_baseName ( (char*)sourceFile, (char*)&node->fileName, MEMORYTRACKER_FILENAME_CHAR_SIZE );
        
        snprintf((char*)&node->functionName, (size_t)MEMORYTRACKER_FUNCTIONNAME_CHAR_SIZE, "%s",sourceFunc);
        
        node->lineNumber    = sourceLine;
        
        node->allocSize     = reqSize;
        node->mallocPointer = mallocPointer;
        
        /* Get the backtrace */      
        node->stackSize = backtrace(node->stacktrace, MEMORYTRACKER_STACKTRACE_ARRAY_SIZE);
    }
    
    return node;
}

MALLOCNODE * memoryTracker_node_getNodeFromPointer ( void * ptr )
{
    return memoryTracker_node_prv_getNodeFromPointer(ptr);
}

bool memoryTracker_node_isNodeValid ( MALLOCNODE * node )
{
    bool isValid = false;

    if ( node )
    {
        void * pre_tag     = memoryTracker_node_prv_getBoundaryPointerPre(node);
        void * post_tag    = memoryTracker_node_prv_getBoundaryPointerPost(node);
        
        if ( memoryTracker_node_prv_isBoundaryTagValid( pre_tag, (uint8_t)MEMORYTRACKER_BOUNDARY_TAGS_PREINSERT ) == false )
        {
            MEMTRK_LOG_E("boundary tag check pre failed\n");
        }
        else if ( memoryTracker_node_prv_isBoundaryTagValid( post_tag, (uint8_t)MEMORYTRACKER_BOUNDARY_TAGS_POSTINSERT ) == false )
        {
            MEMTRK_LOG_E("boundary tag check post failed\n");
        }
        else
        {
            isValid = true;
        }
    }

    return isValid;
}

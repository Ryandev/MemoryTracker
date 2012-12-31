/**
 @file
 Memory Tracker used to track mallocs - common
 
 @author Ryan Powell
 @date 03-10-11
 @copyright
     Copyright (c) 2011  Ryan Powell \n
     Proprietary & Confidential \n
     This file & associated documentation may not be used without the consent of the authors permission. \n
     Undocumented use of this material shall be an infringement of copyright. \n
 */


#ifndef _MEMORYTRACKER_COMMON_H
#define _MEMORYTRACKER_COMMON_H


#ifdef __cplusplus
extern "C" {
#endif

    
#include "rbtree.h"


#define MEMORYTRACKER_FILENAME_CHAR_SIZE (32U)
#define MEMORYTRACKER_FUNCTIONNAME_CHAR_SIZE (32U)
#define MEMORYTRACKER_STACKTRACE_ARRAY_SIZE (32U)


typedef struct _MALLOCNODE
{
    uint8_t stackSize;
    uint16_t lineNumber;
    
    RBTREE_KEY key;

    size_t allocSize;
    void * mallocPointer;
    
    char fileName[MEMORYTRACKER_FILENAME_CHAR_SIZE];
    char functionName[MEMORYTRACKER_FUNCTIONNAME_CHAR_SIZE];
    void * stacktrace[MEMORYTRACKER_STACKTRACE_ARRAY_SIZE];
} MALLOCNODE;

    
/* Enable to trace any defects in MemoryTracker */
/* #define MEMORY_TRACKER_LOGGER */

#ifdef MEMORY_TRACKER_LOGGER
    #include <stdio.h>
    
    #define MEMTRK_LOG(level,format, ... ) \
        do \
        { \
            fprintf (stdout,"MemoryTracker %s (%s:%d): ",level,__FUNCTION__,__LINE__); \
            fprintf (stdout, format, ##__VA_ARGS__ ); \
            fprintf (stdout, "\n"); \
        } while (0)

    #define MEMTRK_LOG_E(fmt, ... ) MEMTRK_LOG("-ERROR-",fmt, ##__VA_ARGS__)
    #define MEMTRK_LOG_I(fmt, ... ) MEMTRK_LOG("-INFO-",fmt, ##__VA_ARGS__)

    #define MEMTRK_ASSERT(cond) \
        if ( (cond) != true ) \
        { \
            MEMTRK_LOG_E("Assertion failure (%s:%d)",__FILE__,__LINE__); \
        }

#else
    #define MEMTRK_LOG_E(fmt, ...) {}
    #define MEMTRK_LOG_I(fmt, ...) {}
    #define MEMTRK_ASSERT(cond) {}
#endif


#ifdef __cplusplus
}
#endif


#endif /* _MEMORYTRACKER_COMMON_H */

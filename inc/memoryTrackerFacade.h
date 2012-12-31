/**
 @file
 Memory tracking/managing libraries
 
 @author Ryan Powell
 @date 03-10-11
 @copyright
     Copyright (c) 2011  Ryan Powell \n
     Proprietary & Confidential \n
     This file & associated documentation may not be used without the consent of the authors permission. \n
     Undocumented use of this material is an infringement of copyright. \n
 */


#ifndef _MEMORYTRACKERFACADE_H
#define _MEMORYTRACKERFACADE_H


#ifdef __cplusplus
extern "C" {
#endif

    
#include "memoryTrackerSettings.h"


/* enable to logger memory usage */
#if MEMORY_USAGE_CHECKER == 1

/**
 Memory Tracker Enabled
 */

#include "MemoryTracker.h"
      

/* wrap to memory-checker tool */

    
/**
 @def memInit
 @brief initialize memory manager
 */
#define memInit memoryTracker_init()
    
    
/**
 @def memTerm
 @brief terminate memory manager
 */
#define memTerm memoryTracker_term()
    
    
/**
 @def memAlloc
 @brief allocate memory & return pointer to address
 @param[in] size number of bytes to allocate
 @return returns non-null on success
 */
#define memAlloc(size) memoryTracker_alloc(__FILE__, __LINE__, __FUNCTION__, (size) )
    
/**
 @def memReAlloc
 @brief Reallocate chunk of memory
 @param[in] ptr Pointer to block of memory
 @param[in] size New size to realloc
 @return returns non-null on success
 */
#define memReAlloc(ptr,size) memoryTracker_realloc(__FILE__, __LINE__, __FUNCTION__, (ptr), (size) )
    
/**
 @def memClearAlloc
 @brief same as #memAlloc except memory is guaranteed to be cleared
 @param[in] size Number of bytes to allocate
 @return returns non-null on success
 */
#define memClearAlloc(size) memoryTracker_alloc(__FILE__, __LINE__, __FUNCTION__, (size) )
    
/**
 @def memFree
 @brief Free memory allocated by #memAlloc or #memClearAlloc or #memReAlloc
 @param[in] ptr Pointer to memory address being freed
 */
#define memFree(ptr) memoryTracker_free(__FILE__, __LINE__, __FUNCTION__, (ptr) )
 
    
/* logger */


/**
 @def memIsPtrValid
 @brief test is memory pointer has been corrupted
 */
#define memIsPtrValid(ptr) memoryTracker_isPointerValid(ptr)
    
/**
 @def memPrintAllocations
 @brief Prints all allocations
 */
#define memPrintAllocations printAllocations()
    
/**
 @def memTotalAllocsBytes
 @brief Returns totals allocated bytes
 @return returns the number of bytes currently allocated
 */
#define memTotalAllocsBytes memoryTracker_allocatedBytes()
    
/**
 @def memTotalAllocsCount
 @brief Returns the number of calls to and/or #memAlloc, #memReAlloc #memClearAlloc
 @return returns allocation count
 */
#define memTotalAllocsCount memoryTracker_allocatedCount()

#else
    
/**
 Memory Tracker Disabled
 */


#include <stdlib.h>


/**
 @def memInit
 @brief initialize memory manager (in this case not)
 */
#define memInit


/**
 @def memTerm
 @brief terminate memory manager (in this case not)
 */
#define memTerm


/**
 @def memAlloc
 @brief wrapper to malloc. To enable memory tracking include define: #MEMORY_USAGE_CHECKER
 */
#define memAlloc(a) malloc((a))


/**
 @def memReAlloc
 @brief wrapper to realloc. To enable memory tracking include define: #MEMORY_USAGE_CHECKER
 */
#define memReAlloc(a,b) realloc((a),(b))


/**
 @def memClearAlloc
 @brief wrapper to calloc. To enable memory tracking include define: #MEMORY_USAGE_CHECKER
 */
#define memClearAlloc(a) calloc(a)


/**
 @def memFree
 @brief wrapper to free. To enable memory tracking include define: #MEMORY_USAGE_CHECKER
 */
#define memFree(a) \
    do \
    { \
        free((a)); \
    } while (0)


/**
 @def memIsPtrValid
 @brief behaviour=NULL. To enable memory tracking include define: #MEMORY_USAGE_CHECKER
 */
#define memIsPtrValid(ptr) true


/**
 @def memTotalAllocsBytes
 @brief behaviour=NULL. To enable memory tracking include define: #MEMORY_USAGE_CHECKER
 */
#define memTotalAllocsBytes 0U


/**
 @def memTotalAllocsCount
 @brief behaviour=NULL. To enable memory tracking include define: #MEMORY_USAGE_CHECKER
 */
#define memTotalAllocsCount 0U


/**
 @def memPrintAllocations
 @brief behaviour=NULL. To enable memory tracking include define: #MEMORY_USAGE_CHECKER
 */
#define memPrintAllocations {}

    
#endif /* MEMORY_USAGE_CHECKER */


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _MEMORYTRACKERFACADE_H */

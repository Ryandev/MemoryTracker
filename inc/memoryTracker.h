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


#ifndef _MEMORYTRACKER_H
#define _MEMORYTRACKER_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


/**
 @brief Initialize Memory Tracker
 @details must be called before calling any other api's
 */
void memoryTracker_init ( void );


/**
 @brief Terminate Memory Tracker
 @details after termination no other Memory Tracker api's can be called other than #memoryTracker_init
 */
void memoryTracker_term ( void );


/**
 @brief Allocate block of memory & add location to tracker
 @param[in] sourceFile Filename on which alloc occured
 @param[in] sourceLine Linenumber on which alloc occured
 @param[in] sourceFunc Function on which alloc occured
 @param[in] reqSize malloc size
 @retval NULL memory allocation failed
 @retval !NULL pointer to allocated memory.
 */
void * memoryTracker_alloc  ( const char *sourceFile, const unsigned int sourceLine, const char *sourceFunc, const size_t reqSize );


/**
 @brief Rellocate block of memory & add location to tracker
 @param[in] sourceFile Filename on which realloc occured
 @param[in] sourceLine Linenumber on which realloc occured
 @param[in] sourceFunc Function on which realloc occured
 @param[in] reqSize realloc size
 @retval NULL memory reallocation failed
 @retval !NULL pointer to reallocated memory.
 */
void * memoryTracker_realloc ( const char *sourceFile, const unsigned int sourceLine, const char *sourceFunc, void * ptr, const size_t reqSize );


/**
 @brief Free block of memory & remove location from tracker
 @param[in] sourceFile Filename on which free occured
 @param[in] sourceLine Linenumber on which free occured
 @param[in] sourceFunc Function on which free occured
 @param[in] memLocation alloc pointer contents to be released
 */
void memoryTracker_free ( const char *sourceFile, const unsigned int sourceLine, const char *sourceFunc, void * memLocation );


/**
 @brief Check if pointer is ok and not corrupted
 @param[in] mem_ptr memory pointer to check
 @retval #true pointer is valid
 @retval #false pointer has been corrupted
 */
bool memoryTracker_isPointerValid ( void * mem_ptr );
    
/**
 @brief Get the total number of bytes not freed
 @return total number of allocated bytes yet to be freed
 */
uint32_t memoryTracker_allocatedBytes ( void );


/**
 @brief Get the total number of allocs not freed
 @return total number of allocs
 */
uint32_t memoryTracker_allocatedCount ( void );


/**
 @brief Print the details of each alloc currently not released to stdout
 */
void printAllocations (void);

    
#ifdef __cplusplus
}
#endif


#endif /* _MEMORYTRACKER_H */

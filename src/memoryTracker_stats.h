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


#ifndef _MEMORYTRACKER_STATS_H
#define _MEMORYTRACKER_STATS_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <stdint.h>

#include "memoryTracker_common.h"


/**
 @brief retrieve the total number of bytes currently allocated that have not been free'd yet
 @return returns byte allocation count
 */
uint32_t memoryTracker_stats_totalBytesAllocated ( void );


/**
 @brief retrieve the total number of alloc requests unfree'd
 @return return allocation count
 */
uint32_t memoryTracker_stats_totalOutstandingAllocs ( void );


/**
 @brief print summary of node
 @param[in] node malloc node to print
 @param[in] reason additional message printing with node as to why it is being printed ( optional )
 @param[in] file file pointer to print summary to
 */
void memoryTracker_stats_printDebugForNode ( MALLOCNODE * node, char * reason, FILE * file );


/**
 @brief print summary of outstanding memory allocations
 @param[in] file file pointer to print summary to
 */
void memoryTracker_stats_printDebug ( FILE * file );


#ifdef __cplusplus
}
#endif


#endif /* _MEMORYTRACKER_STATS_H */

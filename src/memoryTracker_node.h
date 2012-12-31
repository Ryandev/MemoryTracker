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


#ifndef _MEMORYTRACKER_NODE_H
#define _MEMORYTRACKER_NODE_H


#ifdef __cplusplus
extern "C" {
#endif
    

#include "memoryTracker_common.h"


/**
 @brief free node from memory pool
 @param[in] node node to free
 @return returns #true on success
 */
bool memoryTracker_node_remove ( MALLOCNODE * node );


/**
 @brief create node in memory pool
 @param[in] sourceFile file name of allocation occurence
 @param[in] sourceLine line number of allocation occurence
 @param[in] sourceFunc function name of allocation occurence
 @param[in] reqSize number of bytes of allocation
 @param[in] memptr pointer to memory allocation
 @return returns !NULL on success
 */
MALLOCNODE * memoryTracker_node_create ( const char *sourceFile, const unsigned int sourceLine, const char *sourceFunc, const size_t reqSize );


/**
 @brief from a previously created #_MALLOCNODE ( via #MemoryTracker_node_create ) find the user mallocPointer
 @param[in] mallocPointer malloc pointer
 @return returns !NULL on success
 */
MALLOCNODE * memoryTracker_node_getNodeFromPointer ( void * mallocPointer );


/**
 @brief from a given malloc pointer check if the pointer has been corrupted
 @param[in] mallocPointer pointer to check
 @return returns #true on success
 */
bool memoryTracker_node_isNodeValid ( MALLOCNODE * mallocPointer );


#ifdef __cplusplus
}
#endif


#endif /* _MEMORYTRACKER_NODE_H */

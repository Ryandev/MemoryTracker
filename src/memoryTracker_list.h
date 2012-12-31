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


#ifndef _MEMORYTRACKER_LIST_H
#define _MEMORYTRACKER_LIST_H


#ifdef __cplusplus
extern "C" {
#endif
    

#include <stdbool.h>

#include "memoryTracker_common.h"


/**
 @brief initialize list management
 @return #true is initialize successfully
 */
bool memoryTracker_list_init ( void );


/**
 @brief termiante list management
 @return #true is terminated successfully
 */
bool memoryTracker_list_term ( void );


/**
 @brief insert node into list
 @details node->key will be populated upon insertion
 @param[in] node node to insert
 @return #true is inserted successfully
 */
bool memoryTracker_list_insert ( MALLOCNODE * node );


/**
 @brief remove node from list based on node->key
 @param[in] node node to remove
 @return #true is inserted successfully
 */
bool memoryTracker_list_remove ( MALLOCNODE * node );


/**
 @brief find memory pointer (node->mallocPointer)
 @return returns !NULL on success
 */
MALLOCNODE * memoryTracker_list_findMemoryPointer ( void * memptr );


/**
 @brief get entry count
 @return returns list size
 */
uint32_t memoryTracker_list_count ( void );


/**
 @brief get node at index
 @brief index node to retrive
 @return returns !NULL on success
 */
MALLOCNODE * memoryTracker_list_getAtIndex ( uint32_t index );


#ifdef __cplusplus
}
#endif


#endif /* _MEMORYTRACKER_ENTRY_H */

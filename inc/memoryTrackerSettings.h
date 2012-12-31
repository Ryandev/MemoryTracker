/**
 @file
 Memory settings. All configuration settings for memory are here
 
 @author Ryan Powell
 @date 03-10-11
 @copyright
     Copyright (c) 2011  Ryan Powell \n
     Proprietary & Confidential \n
     This file & associated documentation may not be used without the consent of the authors permission. \n
     Undocumented use of this material is an infringement of copyright. \n
 */
 
#ifndef _MEMORYTRACKERSETTINGS_H
#define _MEMORYTRACKERSETTINGS_H


#ifdef __cplusplus
extern "C" {
#endif


/**
 Set to 1 to Enable to track memory usage ( slower - but useful for diagnosing memory issues ) \n
 else default malloc/free will be used
 */
#define MEMORY_USAGE_CHECKER 1


/** settings below only valid if #MEMORY_USAGE_CHECKER is set to 1 */

/** 
 When set to 1 it will print the stack trace as a list of function names. \n
 When set to 0 it will print the stack trace as a list of function pointers \n
 */
#define MEMORYTRACKER_PRINT_STACKTRACE_AS_STRINGS 1


/** number of bytes allocated either side of the memory pointer */
#define MEMORYTRACKER_BOUNDARY_TAGS_SIZE 8U


/** value of tag before user pointer */
#define MEMORYTRACKER_BOUNDARY_TAGS_PREINSERT 0x55555555


/** value of tag after user pointer */
#define MEMORYTRACKER_BOUNDARY_TAGS_POSTINSERT 0xAAAAAAAA


/* default pattern to wipe the user data with */
#define MEMORYTRACKER_USER_WIPE_PATTERN 0xBBBBBBBB

    
#ifdef __cplusplus
}
#endif


#endif /* _MEMORYTRACKERSETTINGS_H */

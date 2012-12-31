
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (c) 2011  Ryan Powell                                       *
 * Proprietary & Confidential                                            *
 * This file & associated documentation may not be used                  *
 * without the consent of the authors permission.                        *
 * Undocumented use this material shall be an infringement of copyright. *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "test_memManager.h"
#include "memoryTrackerFacade.h"


#define RANDOM_UINT32_RANGE(x,y) (uint32_t) ( (rand() % (((y)+1)-(x))) + (x) )


bool test_singleCycle ( void )
{
    uint32_t startAllocBytes = memTotalAllocsBytes;
    uint32_t startAllocCount = memTotalAllocsCount;
    uint32_t allocSize = 36;
    void * memptr = memAlloc(allocSize);
    
    if ( ! memptr )
    {
        printf("Failed malloc\n");
        return false;
    }
    
    if ( memTotalAllocsBytes != allocSize+startAllocBytes )
    {
        printf("Failed alloc malloc mismatch %u!=%u\n",memTotalAllocsBytes,allocSize+startAllocBytes);
        return false;
    }
    
    if ( memTotalAllocsCount != 1+startAllocCount )
    {
        printf("Failed alloc count malloc mismatch %u!=%u\n",memTotalAllocsCount,startAllocCount+1);
        return false;
    }
    
    printf("Allocated:\n");
    memPrintAllocations;
    
    memFree(memptr);

    if ( memTotalAllocsBytes != startAllocBytes )
    {
        printf("Failed alloc free mismatch %u!=%u\n",memTotalAllocsBytes,startAllocBytes);
        return false;
    }
    
    if ( memTotalAllocsCount != startAllocCount )
    {
        printf("Failed alloc count free mismatch %u!=%u\n",memTotalAllocsCount,startAllocCount);
        return false;
    }
    
    printf("Emptied:\n");
    memPrintAllocations;
    
    return true;
}

bool test_multiAlloc ( void )
{
    uint32_t startAllocBytes = memTotalAllocsBytes;
    uint32_t startAllocCount = memTotalAllocsCount;

    uint32_t allocCount = 1024U;
    
    void * allocArray[1024U];
    
    for ( uint32_t i=0U; i<allocCount; i++ )
    {
        size_t allocSize = RANDOM_UINT32_RANGE(10, 1024);
        
        allocArray[i] = memAlloc( allocSize );
        
        if ( allocArray[i] == NULL )
        {
            printf("Multi Alloc failed: (%u)",i);
            return false;
        }
    }

    for ( uint32_t i=0U; i<allocCount; i++ )
    {
        memFree( allocArray[i] );
    }

    if ( memTotalAllocsBytes != startAllocBytes )
    {
        printf("Failed alloc free mismatch %u!=%u\n",memTotalAllocsBytes,startAllocBytes);
        return false;
    }
    
    if ( memTotalAllocsCount != startAllocCount )
    {
        printf("Failed alloc count free mismatch %u!=%u\n",memTotalAllocsCount,startAllocCount);
        return false;
    }

    return true;
}

bool test_realloc ( void )
{
    uint32_t size1 = 10;
    uint32_t size2 = 20;
    uint32_t val = 0x87654321U;

    uint32_t * ptr = memAlloc(size1 * sizeof(uint32_t));
    
    if ( ! ptr )
    {
        printf("malloc failed\n");
        return false;
    }
    
    for (uint32_t i=0; i<size1; i++)
    {
        ptr[i] = val;
    }

    
    for (uint32_t i=0; i<size1-1; i++)
    {
        if ( ptr[i] != val )
        {
            printf("Mismatch between PTR expected %u and actual:%u\n",val,ptr[i]);
            return false;
        }
    }

    uint32_t* ptr_realloc = memReAlloc(ptr,(size2 * sizeof(uint32_t)));
    
    if ( ! ptr_realloc )
    {
        printf("realloc failed\n");
        return false;
    }
    
    for (uint32_t i=0; i<size1; i++)
    {
        if ( ptr_realloc[i] != val )
        {
            printf("Mismatch between expected %u and actual:%u\n",val,ptr_realloc[i]);
            return false;
        }
    }

    for (uint32_t i=size1; i<size2; i++)
    {
        if ( ptr_realloc[i] != MEMORYTRACKER_USER_WIPE_PATTERN )
        {
            printf("Realloc expected %u in realloced unused space\n", MEMORYTRACKER_USER_WIPE_PATTERN);
            return false;
        }
    }
    
    return true;
}

bool test_corruption ( void )
{
    uint32_t size = 32;
    void ** ptr = memAlloc(size);
    void * val = (void*)0xFFFFFFFF;
    
    if ( ptr == NULL )
    {
        printf("malloc failed\n");
        return false;
    }
    
    if ( memIsPtrValid(ptr) == false )
    {
        printf("ptr allocated is not OK?!\n");
        return false;
    }
    
    uint8_t * ptr8bit = (uint8_t *)ptr;
    
    /* write over allocated memory plus 1 */
    for (uint8_t i=0; i<size+1; i++)
    {
        ptr8bit[i] = (uint8_t)val;
    }
    
    if ( memIsPtrValid(ptr) == true )
    {
        printf("ptr allocated is OK after boundary overwrite?!\n");
        return false;
    }

    return true;
}

bool test_allocFree ( uint32_t arraySize )
{
    bool didTestPass = true;
    
    uint32_t storeValue = RANDOM_UINT32_RANGE(1,0xFFFFFFFFU);
    uint32_t * memArray[arraySize];
    
    for ( uint32_t i=0U; i<arraySize; i++ )
    {
        memArray[i] = memAlloc(sizeof(uint32_t));
        
        if ( memArray[i] == NULL )
        {
            printf("Failed to alloc after %u\n",i);
            didTestPass = false;
            break;
        }
        else
        {
            *memArray[i] = storeValue;
        }
    }
    
    /* bail out if its failed */
    if ( didTestPass == false ){ return false; }
    
    /* test store success */
    for ( uint32_t i=0U; i<arraySize; i++ )
    {
        if ( *memArray[i] != storeValue )
        {
            printf("Invalid retrieval %u %u!=%u\n",i,*memArray[i],storeValue);
            didTestPass = false;
            break;
        }
    }
    
    /* bail out if its failed */
    if ( didTestPass == false ){ return false; }
    
    /* free */
    for ( uint32_t i=0U; i<arraySize; i++ )
    {
        memFree(memArray[i]);
    }
    
    return didTestPass;
}

bool test_allocArray ( void )
{
    bool didTestPass = true;
    
    uint8_t loopCount = 0xFFU;
    
    for (uint8_t i=0; i<loopCount; i++ )
    {
        if ( test_allocFree(i) == false )
        {
            printf("Cycle failed after %u\n",i);
            didTestPass = false;
            break;
        }
    }
    
    return didTestPass;
}

void* test_size ( void * size )
{
    void * ret = (void*)1;
    
    if ( test_allocFree((uint32_t)size) )
    {
        ret = (void*)0;
    }
    
    return ret;
}

bool test_concurrency ( uint32_t threadcount )
{
    bool didPass = true;
    
    pthread_attr_t attr;
    pthread_t threads[threadcount];
    int retCode[threadcount];
    
    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for(uint32_t t=0; t<threadcount; t++)
    {
        printf("Creating thread: %u\n",t);

        retCode[t] = pthread_create(&threads[t], NULL, test_size, (void *)256);

        if ( retCode[t] )
        {
            printf("Retcode %d failed\n",t);
            didPass = false;
        }
    }    

    pthread_attr_destroy(&attr);    

    /* wait for the other threads */
    for(uint32_t t=0; t<threadcount; t++)
    {
        void * status = NULL;
        int rc = pthread_join(threads[t], &status);

        if (rc)
        {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            didPass = false;
        }
        else if ( status != NULL )
        {
            printf("Thread %u failed to return \'OK\'\n",t);
        }
        else
        {
            printf("Thread %u returned \'OK\'\n",t);
        }
    }

    return didPass;
}

bool test_memMan ( void )
{

#if MEMORY_USAGE_CHECKER == 0

    printf("CANNOT test without memAlloc enabled\n");
    return false;

#else

    printf("Testing memMan singleCycle\n");
    
    if ( test_singleCycle() == false )
    {
        printf("Test memMan singleCycle failed\n");
        return false;
    }

    printf("Testing memMan singleCycle - repeat\n");

    for ( uint32_t i=0U; i<512U; i++ )
    {
        if ( test_singleCycle() == false )
        {
            printf("Test memMan singleCycle failed\n");
            return false;
        }
    }
    
    printf("Testing memMan multi alloc\n");
    
    if ( test_multiAlloc() == false )
    {
        printf("Test memMan multi alloc failed\n");
        return false;
    }
    
    printf("Testing memMan realloc\n");
    
    if ( test_realloc() == false )
    {
        printf("Test memMan realloc failed\n");
        return false;
    }

    printf("Testing memMan corruption\n");
    
    if ( test_corruption() == false )
    {
        printf("Test memMan corruption failed\n");
        return false;
    }

    printf("Testing memMan allocArray\n");
    
    if ( test_allocArray() == false )
    {
        printf("Test memMan allocArray failed\n");
        return false;
    }
    
    printf("Testing memMan concurrency\n");
    
    if ( test_concurrency(16) == false )
    {
        printf("Test_concurrency failed\n");
        return false;
    }
    
#endif

    printf("Test memMan passed\n");
    
    return true;
}

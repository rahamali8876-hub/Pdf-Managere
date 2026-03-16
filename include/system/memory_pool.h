// include/system/memory_pool.h

#ifndef CLEANER_SYSTEM_MEMORY_POOL_H
#define CLEANER_SYSTEM_MEMORY_POOL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>

#include "platform/platform_api.h"

    /*
    ============================================================
    Memory Pool Allocator

    Fixed-size block allocator designed for high-performance
    systems code.

    Characteristics:
    - O(1) allocation
    - O(1) free
    - lock protected
    - minimal fragmentation
    ============================================================
    */

    typedef struct cleaner_memory_pool_s cleaner_memory_pool_t;

    /* Create memory pool */
    cleaner_memory_pool_t *
    cleaner_memory_pool_create(
        const cleaner_platform_api_t *platform,
        size_t block_size,
        size_t capacity);

    /* Allocate block */
    void *
    cleaner_memory_pool_alloc(cleaner_memory_pool_t *pool);

    /* Free block */
    void
    cleaner_memory_pool_free(cleaner_memory_pool_t *pool, void *ptr);

    /* Destroy pool */
    void
    cleaner_memory_pool_destroy(cleaner_memory_pool_t *pool);

    /* Statistics */

    size_t
    cleaner_memory_pool_capacity(const cleaner_memory_pool_t *pool);

    size_t
    cleaner_memory_pool_block_size(const cleaner_memory_pool_t *pool);

    size_t
    cleaner_memory_pool_free_blocks(const cleaner_memory_pool_t *pool);

#ifdef __cplusplus
}
#endif

#endif
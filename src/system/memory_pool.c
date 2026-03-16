
#include "system/memory_pool.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*
============================================================
Internal Block Structure
============================================================
*/

typedef struct cleaner_memory_block_s
{
    struct cleaner_memory_block_s *next;

} cleaner_memory_block_t;

/*
============================================================
Memory Pool Structure
============================================================
*/

struct cleaner_memory_pool_s
{
    const cleaner_platform_api_t *platform;

    size_t block_size;
    size_t capacity;

    size_t free_count;

    void *buffer;

    cleaner_memory_block_t *free_list;

    cleaner_mutex_t *mutex;
};

/*
============================================================
Utility: Align size
============================================================
*/

static size_t
cleaner_align_size(size_t size)
{
    size_t align = sizeof(void *);

    return (size + align - 1) & ~(align - 1);
}

/*
============================================================
Initialize Free List
============================================================
*/

static void
cleaner_memory_pool_build_freelist(cleaner_memory_pool_t *pool)
{
    char *buffer = (char *)pool->buffer;

    cleaner_memory_block_t *prev = NULL;

    for (size_t i = 0; i < pool->capacity; i++)
    {
        cleaner_memory_block_t *block =
            (cleaner_memory_block_t *)(buffer + i * pool->block_size);

        block->next = prev;

        prev = block;
    }

    pool->free_list = prev;
}

/*
============================================================
Create Pool
============================================================
*/

cleaner_memory_pool_t *
cleaner_memory_pool_create(
    const cleaner_platform_api_t *platform,
    size_t block_size,
    size_t capacity)
{
    if (!platform || block_size == 0 || capacity == 0)
        return NULL;

    cleaner_memory_pool_t *pool =
        (cleaner_memory_pool_t *)malloc(sizeof(cleaner_memory_pool_t));

    if (!pool)
        return NULL;

    memset(pool, 0, sizeof(*pool));

    pool->platform = platform;

    pool->block_size = cleaner_align_size(block_size);

    if (pool->block_size < sizeof(cleaner_memory_block_t))
        pool->block_size = sizeof(cleaner_memory_block_t);

    pool->capacity = capacity;

    pool->free_count = capacity;

    size_t total_size = pool->block_size * capacity;

    pool->buffer = malloc(total_size);

    if (!pool->buffer)
    {
        free(pool);
        return NULL;
    }

    pool->mutex = platform->mutex_create();

    if (!pool->mutex)
    {
        free(pool->buffer);
        free(pool);
        return NULL;
    }

    cleaner_memory_pool_build_freelist(pool);

    return pool;
}

/*
============================================================
Allocate Block
============================================================
*/

void *
cleaner_memory_pool_alloc(cleaner_memory_pool_t *pool)
{
    if (!pool)
        return NULL;

    pool->platform->mutex_lock(pool->mutex);

    cleaner_memory_block_t *block = pool->free_list;

    if (!block)
    {
        pool->platform->mutex_unlock(pool->mutex);
        return NULL;
    }

    pool->free_list = block->next;

    pool->free_count--;

    pool->platform->mutex_unlock(pool->mutex);

    return (void *)block;
}

/*
============================================================
Free Block
============================================================
*/

void cleaner_memory_pool_free(cleaner_memory_pool_t *pool, void *ptr)
{
    if (!pool || !ptr)
        return;

    cleaner_memory_block_t *block =
        (cleaner_memory_block_t *)ptr;

    pool->platform->mutex_lock(pool->mutex);

    block->next = pool->free_list;

    pool->free_list = block;

    pool->free_count++;

    pool->platform->mutex_unlock(pool->mutex);
}

/*
============================================================
Destroy Pool
============================================================
*/

void cleaner_memory_pool_destroy(cleaner_memory_pool_t *pool)
{
    if (!pool)
        return;

    if (pool->mutex)
        pool->platform->mutex_destroy(pool->mutex);

    if (pool->buffer)
        free(pool->buffer);

    free(pool);
}

/*
============================================================
Statistics
============================================================
*/

size_t
cleaner_memory_pool_capacity(const cleaner_memory_pool_t *pool)
{
    return pool ? pool->capacity : 0;
}

size_t
cleaner_memory_pool_block_size(const cleaner_memory_pool_t *pool)
{
    return pool ? pool->block_size : 0;
}

size_t
cleaner_memory_pool_free_blocks(const cleaner_memory_pool_t *pool)
{
    return pool ? pool->free_count : 0;
}
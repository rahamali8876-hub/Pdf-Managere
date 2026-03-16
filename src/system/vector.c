#include "system/vector.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*
============================================================
VECTOR STRUCTURE
============================================================
*/

struct cleaner_vector_s
{
    const cleaner_platform_api_t *platform;

    size_t element_size;
    size_t size;
    size_t capacity;

    uint8_t *data;

    cleaner_mutex_t *mutex;
};

/*
============================================================
UTILITY
============================================================
*/

static size_t next_power_of_two(size_t n)
{
    size_t p = 1;

    while (p < n)
        p <<= 1;

    return p;
}

static int vector_resize(cleaner_vector_t *vec, size_t new_capacity)
{
    if (!vec)
        return -1;

    size_t bytes = new_capacity * vec->element_size;

    uint8_t *new_data = (uint8_t *)realloc(vec->data, bytes);

    if (!new_data)
        return -1;

    vec->data = new_data;
    vec->capacity = new_capacity;

    return 0;
}

/*
============================================================
CREATE
============================================================
*/

cleaner_vector_t *
cleaner_vector_create(
    const cleaner_platform_api_t *platform,
    size_t element_size,
    size_t initial_capacity)
{
    if (!platform || element_size == 0)
        return NULL;

    cleaner_vector_t *vec =
        (cleaner_vector_t *)malloc(sizeof(cleaner_vector_t));

    if (!vec)
        return NULL;

    memset(vec, 0, sizeof(*vec));

    vec->platform = platform;
    vec->element_size = element_size;

    if (initial_capacity == 0)
        initial_capacity = 8;

    initial_capacity = next_power_of_two(initial_capacity);

    vec->capacity = initial_capacity;

    vec->data = malloc(vec->capacity * element_size);

    if (!vec->data)
    {
        free(vec);
        return NULL;
    }

    vec->mutex = platform->mutex_create();

    return vec;
}

/*
============================================================
DESTROY
============================================================
*/

void cleaner_vector_destroy(cleaner_vector_t *vec)
{
    if (!vec)
        return;

    if (vec->mutex)
        vec->platform->mutex_destroy(vec->mutex);

    if (vec->data)
        free(vec->data);

    free(vec);
}

/*
============================================================
SIZE / CAPACITY
============================================================
*/

size_t cleaner_vector_size(const cleaner_vector_t *vec)
{
    return vec ? vec->size : 0;
}

size_t cleaner_vector_capacity(const cleaner_vector_t *vec)
{
    return vec ? vec->capacity : 0;
}

/*
============================================================
RESERVE
============================================================
*/

int cleaner_vector_reserve(cleaner_vector_t *vec, size_t new_capacity)
{
    if (!vec)
        return -1;

    if (new_capacity <= vec->capacity)
        return 0;

    new_capacity = next_power_of_two(new_capacity);

    return vector_resize(vec, new_capacity);
}

/*
============================================================
SHRINK
============================================================
*/

int cleaner_vector_shrink_to_fit(cleaner_vector_t *vec)
{
    if (!vec)
        return -1;

    if (vec->size == vec->capacity)
        return 0;

    size_t new_capacity = next_power_of_two(vec->size);

    return vector_resize(vec, new_capacity);
}

/*
============================================================
ACCESS
============================================================
*/

void *cleaner_vector_at(cleaner_vector_t *vec, size_t index)
{
    if (!vec || index >= vec->size)
        return NULL;

    return vec->data + index * vec->element_size;
}

const void *cleaner_vector_at_const(
    const cleaner_vector_t *vec,
    size_t index)
{
    if (!vec || index >= vec->size)
        return NULL;

    return vec->data + index * vec->element_size;
}

void *cleaner_vector_data(cleaner_vector_t *vec)
{
    return vec ? vec->data : NULL;
}

/*
============================================================
PUSH
============================================================
*/

int cleaner_vector_push_back(
    cleaner_vector_t *vec,
    const void *element)
{
    if (!vec || !element)
        return -1;

    vec->platform->mutex_lock(vec->mutex);

    if (vec->size == vec->capacity)
    {
        size_t new_capacity = vec->capacity * 2;

        if (vector_resize(vec, new_capacity) != 0)
        {
            vec->platform->mutex_unlock(vec->mutex);
            return -1;
        }
    }

    uint8_t *dest =
        vec->data + vec->size * vec->element_size;

    memcpy(dest, element, vec->element_size);

    vec->size++;

    vec->platform->mutex_unlock(vec->mutex);

    return 0;
}

/*
============================================================
POP
============================================================
*/

int cleaner_vector_pop_back(cleaner_vector_t *vec)
{
    if (!vec || vec->size == 0)
        return -1;

    vec->platform->mutex_lock(vec->mutex);

    vec->size--;

    vec->platform->mutex_unlock(vec->mutex);

    return 0;
}

/*
============================================================
INSERT
============================================================
*/

int cleaner_vector_insert(
    cleaner_vector_t *vec,
    size_t index,
    const void *element)
{
    if (!vec || !element)
        return -1;

    if (index > vec->size)
        return -1;

    vec->platform->mutex_lock(vec->mutex);

    if (vec->size == vec->capacity)
    {
        if (vector_resize(vec, vec->capacity * 2) != 0)
        {
            vec->platform->mutex_unlock(vec->mutex);
            return -1;
        }
    }

    uint8_t *pos =
        vec->data + index * vec->element_size;

    memmove(
        pos + vec->element_size,
        pos,
        (vec->size - index) * vec->element_size);

    memcpy(pos, element, vec->element_size);

    vec->size++;

    vec->platform->mutex_unlock(vec->mutex);

    return 0;
}

/*
============================================================
REMOVE
============================================================
*/

int cleaner_vector_remove(
    cleaner_vector_t *vec,
    size_t index)
{
    if (!vec || index >= vec->size)
        return -1;

    vec->platform->mutex_lock(vec->mutex);

    uint8_t *pos =
        vec->data + index * vec->element_size;

    memmove(
        pos,
        pos + vec->element_size,
        (vec->size - index - 1) * vec->element_size);

    vec->size--;

    vec->platform->mutex_unlock(vec->mutex);

    return 0;
}

/*
============================================================
CLEAR
============================================================
*/

void cleaner_vector_clear(cleaner_vector_t *vec)
{
    if (!vec)
        return;

    vec->platform->mutex_lock(vec->mutex);

    vec->size = 0;

    vec->platform->mutex_unlock(vec->mutex);
}

/*
============================================================
COPY
============================================================
*/

int cleaner_vector_copy(
    cleaner_vector_t *dest,
    const cleaner_vector_t *src)
{
    if (!dest || !src)
        return -1;

    if (dest->element_size != src->element_size)
        return -1;

    if (cleaner_vector_reserve(dest, src->size) != 0)
        return -1;

    memcpy(
        dest->data,
        src->data,
        src->size * src->element_size);

    dest->size = src->size;

    return 0;
}

/*
============================================================
SWAP
============================================================
*/

int cleaner_vector_swap(
    cleaner_vector_t *a,
    cleaner_vector_t *b)
{
    if (!a || !b)
        return -1;

    cleaner_vector_t tmp = *a;

    *a = *b;
    *b = tmp;

    return 0;
}
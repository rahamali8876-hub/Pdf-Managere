// include/system/vector.h

#ifndef CLEANER_SYSTEM_VECTOR_H
#define CLEANER_SYSTEM_VECTOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>
#include "platform/platform_api.h"

    /*
    ============================================================
    GENERIC VECTOR CONTAINER

    Dynamic contiguous array supporting generic element types.

    Characteristics
    ---------------
    • O(1) random access
    • O(1) amortized push
    • contiguous memory (cache friendly)
    • supports any data type
    ============================================================
    */

    typedef struct cleaner_vector_s cleaner_vector_t;

    /*==========================================================
    CREATION / DESTRUCTION
    ==========================================================*/

    cleaner_vector_t *
    cleaner_vector_create(
        const cleaner_platform_api_t *platform,
        size_t element_size,
        size_t initial_capacity);

    void
    cleaner_vector_destroy(
        cleaner_vector_t *vec);

    /*==========================================================
    CAPACITY
    ==========================================================*/

    size_t
    cleaner_vector_size(
        const cleaner_vector_t *vec);

    size_t
    cleaner_vector_capacity(
        const cleaner_vector_t *vec);

    int
    cleaner_vector_reserve(
        cleaner_vector_t *vec,
        size_t new_capacity);

    int
    cleaner_vector_shrink_to_fit(
        cleaner_vector_t *vec);

    /*==========================================================
    ELEMENT ACCESS
    ==========================================================*/

    void *
    cleaner_vector_at(
        cleaner_vector_t *vec,
        size_t index);

    const void *
    cleaner_vector_at_const(
        const cleaner_vector_t *vec,
        size_t index);

    void *
    cleaner_vector_data(
        cleaner_vector_t *vec);

    /*==========================================================
    MODIFIERS
    ==========================================================*/

    int
    cleaner_vector_push_back(
        cleaner_vector_t *vec,
        const void *element);

    int
    cleaner_vector_pop_back(
        cleaner_vector_t *vec);

    int
    cleaner_vector_insert(
        cleaner_vector_t *vec,
        size_t index,
        const void *element);

    int
    cleaner_vector_remove(
        cleaner_vector_t *vec,
        size_t index);

    void
    cleaner_vector_clear(
        cleaner_vector_t *vec);

    /*==========================================================
    UTILITY
    ==========================================================*/

    int
    cleaner_vector_copy(
        cleaner_vector_t *dest,
        const cleaner_vector_t *src);

    int
    cleaner_vector_swap(
        cleaner_vector_t *a,
        cleaner_vector_t *b);

#ifdef __cplusplus
}
#endif

#endif
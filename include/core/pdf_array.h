#ifndef PDF_ARRAY_H
#define PDF_ARRAY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include "core/pdf_object.h"

    typedef struct pdf_array_s pdf_array_t;

    /*
    ============================================================
    CREATE / DESTROY
    ============================================================
    */

    pdf_array_t *
    pdf_array_create(void);

    void
    pdf_array_destroy(
        pdf_array_t *arr);

    /*
    ============================================================
    ELEMENTS
    ============================================================
    */

    int
    pdf_array_push(
        pdf_array_t *arr,
        pdf_object_t *obj);

    pdf_object_t *
    pdf_array_get(
        pdf_array_t *arr,
        size_t index);

    size_t
    pdf_array_size(
        const pdf_array_t *arr);

#ifdef __cplusplus
}
#endif

#endif
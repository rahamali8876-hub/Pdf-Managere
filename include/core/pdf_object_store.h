// include/core/pdf_object_store.h"

#ifndef PDF_OBJECT_STORE_H
#define PDF_OBJECT_STORE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include "core/pdf_object.h"

    typedef struct pdf_object_store_s pdf_object_store_t;

    /*
    ============================================================
    CREATE / DESTROY
    ============================================================
    */

    pdf_object_store_t *
    pdf_object_store_create(void);

    void
    pdf_object_store_destroy(
        pdf_object_store_t *store);

    /*
    ============================================================
    OBJECT MANAGEMENT
    ============================================================
    */

    int
    pdf_object_store_add(
        pdf_object_store_t *store,
        pdf_object_t *obj);

    pdf_object_t *
    pdf_object_store_get(
        pdf_object_store_t *store,
        int object_number);

    size_t
    pdf_object_store_count(
        const pdf_object_store_t *store);

    /*
    ============================================================
    ITERATION
    ============================================================
    */

    pdf_object_t *
    pdf_object_store_at(
        pdf_object_store_t *store,
        size_t index);

#ifdef __cplusplus
}
#endif

#endif
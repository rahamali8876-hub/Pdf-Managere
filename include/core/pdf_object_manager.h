#ifndef PDF_CORE_OBJECT_MANAGER_H
#define PDF_CORE_OBJECT_MANAGER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "core/pdf_xref.h"

    /*
    ============================================================
    PDF OBJECT ID
    ============================================================
    */

    typedef int pdf_object_id;

    /*
    ============================================================
    OBJECT MANAGER
    ============================================================
    */

    typedef struct pdf_object_manager_s pdf_object_manager_t;

    /*
    ============================================================
    CREATE / DESTROY
    ============================================================
    */

    pdf_object_manager_t *
    pdf_object_manager_create(size_t initial_capacity);

    void
    pdf_object_manager_destroy(pdf_object_manager_t *mgr);

    /*
    ============================================================
    OBJECT ALLOCATION
    ============================================================
    */

    pdf_object_id
    pdf_object_manager_new(pdf_object_manager_t *mgr);

    /*
    ============================================================
    OBJECT BEGIN
    ============================================================
    */

    int
    pdf_object_manager_begin(
        pdf_object_manager_t *mgr,
        FILE *file,
        pdf_object_id id);

    /*
    ============================================================
    OBJECT END
    ============================================================
    */

    int
    pdf_object_manager_end(FILE *file);

    /*
    ============================================================
    WRITE XREF
    ============================================================
    */

    int
    pdf_object_manager_write_xref(
        pdf_object_manager_t *mgr,
        FILE *file,
        pdf_object_id root_object);

#ifdef __cplusplus
}
#endif

#endif
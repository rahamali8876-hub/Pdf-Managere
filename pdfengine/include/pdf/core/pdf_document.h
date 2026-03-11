

#ifndef PDF_CORE_DOCUMENT_H
#define PDF_CORE_DOCUMENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include "pdf/core/pdf_page.h"

    /*
    ============================================================
    PDF DOCUMENT (Opaque)
    ============================================================
    */

    typedef struct pdf_document_s pdf_document_t;

    /*
    ============================================================
    DOCUMENT LIFETIME
    ============================================================
    */

    pdf_document_t *
    pdf_document_create(void);

    void
    pdf_document_destroy(pdf_document_t *doc);

    /*
    ============================================================
    PAGE MANAGEMENT
    ============================================================
    */

    pdf_page_t *
    pdf_document_add_page(
        pdf_document_t *doc,
        int width,
        int height);

    size_t
    pdf_document_page_count(
        const pdf_document_t *doc);

    pdf_page_t *
    pdf_document_get_page(
        pdf_document_t *doc,
        size_t index);

    /*
    ============================================================
    FILE OUTPUT
    ============================================================
    */

    int
    pdf_document_save(
        pdf_document_t *doc,
        const char *path);

#ifdef __cplusplus
}
#endif

#endif
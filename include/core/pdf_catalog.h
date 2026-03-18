#ifndef PDF_CORE_CATALOG_H
#define PDF_CORE_CATALOG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "core/pdf_pages_tree.h"

    typedef struct pdf_catalog_s pdf_catalog_t;

    /*
    ============================================================
    CREATE / DESTROY
    ============================================================
    */

    pdf_catalog_t *
    pdf_catalog_create(
        pdf_pages_tree_t *pages);

    void
    pdf_catalog_destroy(
        pdf_catalog_t *catalog);

    /*
    ============================================================
    ACCESS
    ============================================================
    */

    pdf_pages_tree_t *
    pdf_catalog_pages(
        pdf_catalog_t *catalog);

#ifdef __cplusplus
}
#endif

#endif
// core/pdf_page_tree.h 

#ifndef PDF_CORE_PAGES_TREE_H
#define PDF_CORE_PAGES_TREE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "core/pdf_page.h"
#include <stddef.h>

    typedef struct pdf_pages_tree_s pdf_pages_tree_t;

    /*
    ============================================================
    CREATE / DESTROY
    ============================================================
    */

    pdf_pages_tree_t *
    pdf_pages_tree_create(void);

    void
    pdf_pages_tree_destroy(pdf_pages_tree_t *tree);

    /*
    ============================================================
    PAGE MANAGEMENT
    ============================================================
    */

    int
    pdf_pages_tree_add_page(
        pdf_pages_tree_t *tree,
        pdf_page_t *page);

    size_t
    pdf_pages_tree_count(
        const pdf_pages_tree_t *tree);

    pdf_page_t *
    pdf_pages_tree_get(
        pdf_pages_tree_t *tree,
        size_t index);

#ifdef __cplusplus
}
#endif

#endif
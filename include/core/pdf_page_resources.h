#ifndef PDF_CORE_PAGE_RESOURCES_H
#define PDF_CORE_PAGE_RESOURCES_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>

    typedef struct pdf_page_resources_s pdf_page_resources_t;

    /*
    ============================================================
    CREATE / DESTROY
    ============================================================
    */

    pdf_page_resources_t *
    pdf_page_resources_create(void);

    void
    pdf_page_resources_destroy(
        pdf_page_resources_t *res);

    /*
    ============================================================
    REGISTER IMAGE (XOBJECT)
    ============================================================
    Returns generated name like "Im0"
    ============================================================
    */

    const char *
    pdf_page_resources_add_image(
        pdf_page_resources_t *res,
        int object_number);

    /*
    ============================================================
    WRITE RESOURCES OBJECT
    ============================================================
    */

    int
    pdf_page_resources_write(
        pdf_page_resources_t *res,
        FILE *file);

    /*
    ============================================================
    COUNT
    ============================================================
    */

    int
    pdf_page_resources_count(
        const pdf_page_resources_t *res);

#ifdef __cplusplus
}
#endif

#endif


#ifndef PDF_CORE_PAGE_H
#define PDF_CORE_PAGE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

    /*
    ============================================================
    PDF PAGE (Opaque)
    ============================================================
    */

    typedef struct pdf_page_s pdf_page_t;

    /*
    ============================================================
    PAGE INFO
    ============================================================
    */

    typedef struct
    {
        int width;
        int height;

    } pdf_page_info_t;

    /*
    ============================================================
    INTERNAL CORE API
    ============================================================
    */

    pdf_page_t *
    pdf_page_create(int width, int height);

    void
    pdf_page_destroy(pdf_page_t *page);

    int
    pdf_page_width(const pdf_page_t *page);

    int
    pdf_page_height(const pdf_page_t *page);

#ifdef __cplusplus
}
#endif

#endif


#ifndef PDF_ENGINE_API_H
#define PDF_ENGINE_API_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>

    /*
    ============================================================
    PDF ENGINE ABI VERSION
    ============================================================
    */

#define PDF_ENGINE_ABI_VERSION 1

    /*
    ============================================================
    OPAQUE TYPES
    ============================================================
    */

    typedef struct pdf_document_s pdf_document_t;
    typedef struct pdf_page_s pdf_page_t;

    /*
    ============================================================
    ENGINE API STRUCT
    ============================================================
    */

    typedef struct pdf_engine_api_s
    {
        uint32_t abi_version;

        /*
        --------------------------------------------------------
        DOCUMENT
        --------------------------------------------------------
        */

        pdf_document_t *(*document_create)(void);

        void (*document_destroy)(
            pdf_document_t *doc);

        /*
        --------------------------------------------------------
        PAGE
        --------------------------------------------------------
        */

        pdf_page_t *(*document_add_page)(
            pdf_document_t *doc,
            int width,
            int height);

        size_t (*document_page_count)(
            const pdf_document_t *doc);

        pdf_page_t *(*document_get_page)(
            pdf_document_t *doc,
            size_t index);

        /*
        --------------------------------------------------------
        SAVE
        --------------------------------------------------------
        */

        int (*document_save)(
            pdf_document_t *doc,
            const char *path);

    } pdf_engine_api_t;

    /*
    ============================================================
    API ACCESSOR
    ============================================================
    */

    const pdf_engine_api_t *
    pdf_engine_get_api(void);

#ifdef __cplusplus
}
#endif

#endif
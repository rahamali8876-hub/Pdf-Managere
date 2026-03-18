// src/api/pdf_engine_api.h

#include "core/pdf_document.h"
#include "api/pdf_engine_api.h"

static pdf_document_t *api_document_create(void)
{
    return pdf_document_create();
}

static void api_document_destroy(pdf_document_t *doc)
{
    pdf_document_destroy(doc);
}

static pdf_page_t *api_document_add_page(
    pdf_document_t *doc,
    int width,
    int height)
{
    // return pdf_document_add_page(doc, width, height);
    return api_document_add_page(doc, width, height);
}

static size_t api_document_page_count(
    const pdf_document_t *doc)
{
    // return pdf_document_page_count(doc);
    return api_document_page_count(doc);
}

static pdf_page_t *api_document_get_page(
    pdf_document_t *doc,
    size_t index)
{
    // return pdf_document_get_page(doc, index);
    return api_document_get_page(doc, index);
}

static int api_document_save(
    pdf_document_t *doc,
    const char *path)
{
    return pdf_document_save(doc, path);
}

/*
============================================================
GLOBAL API INSTANCE
============================================================
*/

static const pdf_engine_api_t g_api =
    {
        PDF_ENGINE_ABI_VERSION,

        api_document_create,
        api_document_destroy,

        api_document_add_page,
        api_document_page_count,
        api_document_get_page,

        api_document_save};

const pdf_engine_api_t *
pdf_engine_get_api(void)
{
    return &g_api;
}
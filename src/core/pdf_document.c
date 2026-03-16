// src/core/pdf_document.c

#include "core/pdf_document.h"
#include "system/vector.h"

#include <stdlib.h>
#include <stdio.h>

/*
============================================================
DOCUMENT STRUCTURE
============================================================
*/

struct pdf_document_s
{
    cleaner_vector_t *pages;
};

/*
============================================================
CREATE
============================================================
*/

pdf_document_t *
pdf_document_create(void)
{
    pdf_document_t *doc =
        (pdf_document_t *)malloc(sizeof(pdf_document_t));

    if (!doc)
        return NULL;

    doc->pages =
        cleaner_vector_create(NULL, sizeof(pdf_page_t *), 8);

    if (!doc->pages)
    {
        free(doc);
        return NULL;
    }

    return doc;
}

/*
============================================================
DESTROY
============================================================
*/

void pdf_document_destroy(pdf_document_t *doc)
{
    if (!doc)
        return;

    size_t count =
        cleaner_vector_size(doc->pages);

    for (size_t i = 0; i < count; i++)
    {
        pdf_page_t **p =
            cleaner_vector_at(doc->pages, i);

        if (p && *p)
            pdf_page_destroy(*p);
    }

    cleaner_vector_destroy(doc->pages);

    free(doc);
}

/*
============================================================
ADD PAGE
============================================================
*/

pdf_page_t *
pdf_document_add_page(
    pdf_document_t *doc,
    int width,
    int height)
{
    if (!doc)
        return NULL;

    pdf_page_t *page =
        pdf_page_create(width, height);

    if (!page)
        return NULL;

    cleaner_vector_push_back(
        doc->pages,
        &page);

    return page;
}

/*
============================================================
PAGE COUNT
============================================================
*/

size_t
pdf_document_page_count(
    const pdf_document_t *doc)
{
    if (!doc)
        return 0;

    return cleaner_vector_size(doc->pages);
}

/*
============================================================
GET PAGE
============================================================
*/

pdf_page_t *
pdf_document_get_page(
    pdf_document_t *doc,
    size_t index)
{
    if (!doc)
        return NULL;

    pdf_page_t **page =
        cleaner_vector_at(doc->pages, index);

    if (!page)
        return NULL;

    return *page;
}

/*
============================================================
SAVE (MINIMAL PDF WRITER)
============================================================
*/

int pdf_document_save(
    pdf_document_t *doc,
    const char *path)
{
    if (!doc || !path)
        return -1;

    FILE *f = fopen(path, "wb");
    if (!f)
        return -1;

    size_t page_count =
        pdf_document_page_count(doc);

    if (page_count == 0)
    {
        fclose(f);
        return -1;
    }

    long *offsets =
        malloc(sizeof(long) * (page_count + 3));

    fprintf(f, "%%PDF-1.4\n");

    /*
    ----------------------------------------
    1 Catalog
    ----------------------------------------
    */

    offsets[1] = ftell(f);

    fprintf(f,
            "1 0 obj\n"
            "<< /Type /Catalog /Pages 2 0 R >>\n"
            "endobj\n");

    /*
    ----------------------------------------
    2 Pages Tree
    ----------------------------------------
    */

    offsets[2] = ftell(f);

    fprintf(f,
            "2 0 obj\n"
            "<< /Type /Pages\n"
            "/Count %zu\n"
            "/Kids [",
            page_count);

    for (size_t i = 0; i < page_count; i++)
    {
        fprintf(f, "%zu 0 R ", i + 3);
    }

    fprintf(f,
            "] >>\n"
            "endobj\n");

    /*
    ----------------------------------------
    PAGE OBJECTS
    ----------------------------------------
    */

    for (size_t i = 0; i < page_count; i++)
    {
        pdf_page_t *page =
            pdf_document_get_page(doc, i);

        offsets[i + 3] = ftell(f);

        fprintf(f,
                "%zu 0 obj\n"
                "<< /Type /Page\n"
                "/Parent 2 0 R\n"
                "/MediaBox [0 0 %d %d]\n"
                ">>\n"
                "endobj\n",
                i + 3,
                pdf_page_width(page),
                pdf_page_height(page));
    }

    /*
    ----------------------------------------
    XREF
    ----------------------------------------
    */

    long xref_pos = ftell(f);

    fprintf(f, "xref\n");
    fprintf(f, "0 %zu\n", page_count + 3);

    fprintf(f, "0000000000 65535 f \n");

    for (size_t i = 1; i < page_count + 3; i++)
    {
        fprintf(f,
                "%010ld 00000 n \n",
                offsets[i]);
    }

    /*
    ----------------------------------------
    TRAILER
    ----------------------------------------
    */

    fprintf(f,
            "trailer\n"
            "<< /Size %zu\n"
            "/Root 1 0 R >>\n",
            page_count + 3);

    fprintf(f,
            "startxref\n"
            "%ld\n"
            "%%%%EOF",
            xref_pos);

    free(offsets);

    fclose(f);

    return 0;
}
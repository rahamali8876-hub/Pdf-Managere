// src/core/pdf_page.c

#include "core/pdf_page.h"

#include <stdlib.h>
#include <string.h>

/*
============================================================
PAGE STRUCTURE
============================================================
*/

struct pdf_page_s
{
    int width;
    int height;
};

/*
============================================================
CREATE
============================================================
*/

pdf_page_t *
pdf_page_create(int width, int height)
{
    if (width <= 0 || height <= 0)
        return NULL;

    pdf_page_t *page =
        (pdf_page_t *)malloc(sizeof(pdf_page_t));

    if (!page)
        return NULL;

    page->width = width;
    page->height = height;

    return page;
}

/*
============================================================
DESTROY
============================================================
*/

void pdf_page_destroy(pdf_page_t *page)
{
    if (!page)
        return;

    free(page);
}

/*
============================================================
ACCESSORS
============================================================
*/

int pdf_page_width(const pdf_page_t *page)
{
    return page ? page->width : 0;
}

int pdf_page_height(const pdf_page_t *page)
{
    return page ? page->height : 0;
}
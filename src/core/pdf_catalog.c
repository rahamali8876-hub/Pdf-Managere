#include "core/pdf_catalog.h"

#include <stdlib.h>

struct pdf_catalog_s
{
    pdf_pages_tree_t *pages;
};

/*
============================================================
CREATE
============================================================
*/

pdf_catalog_t *
pdf_catalog_create(
    pdf_pages_tree_t *pages)
{
    if (!pages)
        return NULL;

    pdf_catalog_t *c =
        malloc(sizeof(pdf_catalog_t));

    if (!c)
        return NULL;

    c->pages = pages;

    return c;
}

/*
============================================================
DESTROY
============================================================
*/

void pdf_catalog_destroy(
    pdf_catalog_t *catalog)
{
    if (!catalog)
        return;

    free(catalog);
}

/*
============================================================
ACCESS
============================================================
*/

pdf_pages_tree_t *
pdf_catalog_pages(
    pdf_catalog_t *catalog)
{
    if (!catalog)
        return NULL;

    return catalog->pages;
}
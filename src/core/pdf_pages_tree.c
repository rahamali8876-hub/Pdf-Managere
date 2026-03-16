// src/core/pdf_pages_tree.c

#include "core/pdf_pages_tree.h"
#include "system/vector.h"

#include <stdlib.h>

struct pdf_pages_tree_s
{
    cleaner_vector_t *pages;
};

/*
============================================================
CREATE
============================================================
*/

pdf_pages_tree_t *
pdf_pages_tree_create(void)
{
    pdf_pages_tree_t *tree =
        malloc(sizeof(pdf_pages_tree_t));

    if (!tree)
        return NULL;

    tree->pages =
        cleaner_vector_create(
            NULL,
            sizeof(pdf_page_t *),
            8);

    return tree;
}

/*
============================================================
DESTROY
============================================================
*/

void pdf_pages_tree_destroy(pdf_pages_tree_t *tree)
{
    if (!tree)
        return;

    cleaner_vector_destroy(tree->pages);

    free(tree);
}

/*
============================================================
ADD PAGE
============================================================
*/

int pdf_pages_tree_add_page(
    pdf_pages_tree_t *tree,
    pdf_page_t *page)
{
    if (!tree || !page)
        return -1;

    return cleaner_vector_push_back(
        tree->pages,
        &page);
}

/*
============================================================
COUNT
============================================================
*/

size_t
pdf_pages_tree_count(
    const pdf_pages_tree_t *tree)
{
    if (!tree)
        return 0;

    return cleaner_vector_size(tree->pages);
}

/*
============================================================
GET PAGE
============================================================
*/

pdf_page_t *
pdf_pages_tree_get(
    pdf_pages_tree_t *tree,
    size_t index)
{
    if (!tree)
        return NULL;

    pdf_page_t **p =
        cleaner_vector_at(tree->pages, index);

    if (!p)
        return NULL;

    return *p;
}
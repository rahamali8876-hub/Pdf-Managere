#include "core/pdf_object_manager.h"

#include <stdlib.h>
#include <stdio.h>

struct pdf_object_manager_s
{
    int next_object;

    pdf_xref_t *xref;
};

/*
============================================================
CREATE
============================================================
*/

pdf_object_manager_t *
pdf_object_manager_create(size_t initial_capacity)
{
    pdf_object_manager_t *mgr =
        malloc(sizeof(pdf_object_manager_t));

    if (!mgr)
        return NULL;

    mgr->next_object = 1;

    mgr->xref =
        pdf_xref_create(initial_capacity);

    if (!mgr->xref)
    {
        free(mgr);
        return NULL;
    }

    return mgr;
}

/*
============================================================
DESTROY
============================================================
*/

void pdf_object_manager_destroy(pdf_object_manager_t *mgr)
{
    if (!mgr)
        return;

    pdf_xref_destroy(mgr->xref);

    free(mgr);
}

/*
============================================================
NEW OBJECT
============================================================
*/

pdf_object_id
pdf_object_manager_new(pdf_object_manager_t *mgr)
{
    if (!mgr)
        return 0;

    return mgr->next_object++;
}

/*
============================================================
BEGIN OBJECT
============================================================
*/

int pdf_object_manager_begin(
    pdf_object_manager_t *mgr,
    FILE *file,
    pdf_object_id id)
{
    if (!mgr || !file)
        return -1;

    long offset = ftell(file);

    pdf_xref_register(
        mgr->xref,
        id,
        offset);

    fprintf(file,
            "%d 0 obj\n",
            id);

    return 0;
}

/*
============================================================
END OBJECT
============================================================
*/

int pdf_object_manager_end(FILE *file)
{
    if (!file)
        return -1;

    fprintf(file,
            "endobj\n");

    return 0;
}

/*
============================================================
WRITE XREF + TRAILER
============================================================
*/

int pdf_object_manager_write_xref(
    pdf_object_manager_t *mgr,
    FILE *file,
    pdf_object_id root_object)
{
    if (!mgr || !file)
        return -1;

    long pos = ftell(file);

    pdf_xref_write(
        mgr->xref,
        file,
        pos);

    fprintf(file,
            "trailer\n"
            "<< /Size %d\n"
            "/Root %d 0 R >>\n",
            mgr->next_object,
            root_object);

    fprintf(file,
            "startxref\n"
            "%ld\n"
            "%%%%EOF\n",
            pos);

    return 0;
}
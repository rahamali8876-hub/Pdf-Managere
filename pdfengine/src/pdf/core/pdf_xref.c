#include "pdf/core/pdf_xref.h"
#include "system/vector.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    int object_number;
    long offset;

} pdf_xref_entry_t;

struct pdf_xref_s
{
    cleaner_vector_t *entries;
};

/*
============================================================
CREATE
============================================================
*/

pdf_xref_t *
pdf_xref_create(size_t initial_capacity)
{
    pdf_xref_t *x =
        malloc(sizeof(pdf_xref_t));

    if (!x)
        return NULL;

    x->entries =
        cleaner_vector_create(
            NULL,
            sizeof(pdf_xref_entry_t),
            initial_capacity);

    return x;
}

/*
============================================================
DESTROY
============================================================
*/

void pdf_xref_destroy(pdf_xref_t *xref)
{
    if (!xref)
        return;

    cleaner_vector_destroy(xref->entries);

    free(xref);
}

/*
============================================================
REGISTER OBJECT
============================================================
*/

int pdf_xref_register(
    pdf_xref_t *xref,
    int object_number,
    long offset)
{
    if (!xref)
        return -1;

    pdf_xref_entry_t e;

    e.object_number = object_number;
    e.offset = offset;

    return cleaner_vector_push_back(
        xref->entries,
        &e);
}

/*
============================================================
WRITE XREF TABLE
============================================================
*/

int pdf_xref_write(
    pdf_xref_t *xref,
    FILE *file,
    long xref_position)
{
    if (!xref || !file)
        return -1;

    size_t count =
        cleaner_vector_size(xref->entries);

    fprintf(file, "xref\n");
    fprintf(file, "0 %zu\n", count + 1);

    fprintf(file, "0000000000 65535 f \n");

    for (size_t i = 0; i < count; i++)
    {
        pdf_xref_entry_t *e =
            cleaner_vector_at(xref->entries, i);

        fprintf(
            file,
            "%010ld 00000 n \n",
            e->offset);
    }

    fprintf(file, "trailer\n");
    fprintf(file, "<< /Size %zu >>\n", count + 1);
    fprintf(file, "startxref\n");
    fprintf(file, "%ld\n", xref_position);
    fprintf(file, "%%EOF\n");

    return 0;
}
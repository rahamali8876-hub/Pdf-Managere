#include "writer/pdf_xref_writer.h"
#include <stdlib.h>

typedef struct
{
    long offset;

} pdf_xref_entry_t;

struct pdf_xref_writer_s
{
    FILE *file;

    pdf_xref_entry_t *entries;

    size_t capacity;
};

pdf_xref_writer_t *
pdf_xref_writer_create(FILE *file)
{
    pdf_xref_writer_t *writer =
        malloc(sizeof(pdf_xref_writer_t));

    if (!writer)
        return NULL;

    writer->file = file;
    writer->capacity = 1024;

    writer->entries =
        calloc(writer->capacity,
               sizeof(pdf_xref_entry_t));

    return writer;
}

void pdf_xref_writer_destroy(
    pdf_xref_writer_t *writer)
{
    if (!writer)
        return;

    free(writer->entries);

    free(writer);
}

void pdf_xref_record_offset(
    pdf_xref_writer_t *writer,
    size_t object_number,
    long offset)
{
    if (object_number >= writer->capacity)
        return;

    writer->entries[object_number].offset = offset;
}

int pdf_xref_write_table(
    pdf_xref_writer_t *writer,
    size_t object_count,
    size_t root_object)
{
    FILE *f = writer->file;

    long xref_offset = ftell(f);

    fprintf(f, "xref\n");
    fprintf(f, "0 %zu\n", object_count + 1);

    /* free object entry */
    fprintf(f, "%010d %05d f \n", 0, 65535);

    for (size_t i = 1; i <= object_count; i++)
    {
        long off = writer->entries[i].offset;

        fprintf(
            f,
            "%010ld %05d n \n",
            off,
            0);
    }

    /* trailer */

    fprintf(f, "trailer\n");
    fprintf(f, "<<\n");
    fprintf(f, "/Size %zu\n", object_count + 1);
    fprintf(f, "/Root %zu 0 R\n", root_object);
    fprintf(f, ">>\n");

    fprintf(f, "startxref\n");
    fprintf(f, "%ld\n", xref_offset);
    fprintf(f, "%%%%EOF\n");

    return 0;
}
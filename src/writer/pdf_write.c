#include "pdf/write/pdf_write.h"

#include <stdlib.h>
#include <stdio.h>

struct pdf_writer_s
{
    FILE *file;
};

/*
============================================================
CREATE
============================================================
*/

pdf_writer_t *
pdf_writer_create(FILE *file)
{
    if (!file)
        return NULL;

    pdf_writer_t *w =
        malloc(sizeof(pdf_writer_t));

    w->file = file;

    return w;
}

/*
============================================================
DESTROY
============================================================
*/

void pdf_writer_destroy(pdf_writer_t *writer)
{
    if (!writer)
        return;

    free(writer);
}

/*
============================================================
WRITE OBJECT
============================================================
*/

long pdf_writer_begin_object(
    pdf_writer_t *writer,
    int object_number)
{
    if (!writer)
        return -1;

    long pos = ftell(writer->file);

    fprintf(
        writer->file,
        "%d 0 obj\n",
        object_number);

    return pos;
}

int pdf_writer_end_object(
    pdf_writer_t *writer)
{
    if (!writer)
        return -1;

    fprintf(
        writer->file,
        "endobj\n");

    return 0;
}

/*
============================================================
WRITE TEXT
============================================================
*/

int pdf_writer_write_string(
    pdf_writer_t *writer,
    const char *text)
{
    if (!writer || !text)
        return -1;

    fprintf(writer->file, "%s", text);

    return 0;
}

/*
============================================================
POSITION
============================================================
*/

long pdf_writer_position(
    pdf_writer_t *writer)
{
    if (!writer)
        return -1;

    return ftell(writer->file);
}
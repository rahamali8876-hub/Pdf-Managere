#ifndef PDF_CORE_WRITER_H
#define PDF_CORE_WRITER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdio.h>

    typedef struct pdf_writer_s pdf_writer_t;

    /*
    ============================================================
    WRITER
    ============================================================
    */

    pdf_writer_t *
    pdf_writer_create(FILE *file);

    void
    pdf_writer_destroy(pdf_writer_t *writer);

    /*
    ============================================================
    OBJECT WRITING
    ============================================================
    */

    long
    pdf_writer_begin_object(
        pdf_writer_t *writer,
        int object_number);

    int
    pdf_writer_end_object(
        pdf_writer_t *writer);

    int
    pdf_writer_write_string(
        pdf_writer_t *writer,
        const char *text);

    long
    pdf_writer_position(
        pdf_writer_t *writer);

#ifdef __cplusplus
}
#endif

#endif
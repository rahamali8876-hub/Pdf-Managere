#ifndef PDF_XREF_WRITER_H
#define PDF_XREF_WRITER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stddef.h>

    typedef struct pdf_xref_writer_s pdf_xref_writer_t;

    /*
    ============================================================
    CREATE / DESTROY
    ============================================================
    */

    pdf_xref_writer_t *
    pdf_xref_writer_create(FILE *file);

    void
    pdf_xref_writer_destroy(
        pdf_xref_writer_t *writer);

    /*
    ============================================================
    OFFSET RECORDING
    ============================================================
    */

    void
    pdf_xref_record_offset(
        pdf_xref_writer_t *writer,
        size_t object_number,
        long offset);

    /*
    ============================================================
    WRITE XREF + TRAILER
    ============================================================
    */

    int
    pdf_xref_write_table(
        pdf_xref_writer_t *writer,
        size_t object_count,
        size_t root_object);

#ifdef __cplusplus
}
#endif

#endif
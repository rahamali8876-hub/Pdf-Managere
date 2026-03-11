#ifndef PDF_CORE_STREAM_H
#define PDF_CORE_STREAM_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>

    typedef struct pdf_stream_s pdf_stream_t;

    /*
    ============================================================
    CREATE / DESTROY
    ============================================================
    */

    pdf_stream_t *
    pdf_stream_create(void);

    void
    pdf_stream_destroy(pdf_stream_t *stream);

    /*
    ============================================================
    WRITE DATA
    ============================================================
    */

    int
    pdf_stream_write(
        pdf_stream_t *stream,
        const void *data,
        size_t size);

    /*
    ============================================================
    SIZE
    ============================================================
    */

    size_t
    pdf_stream_size(
        const pdf_stream_t *stream);

    /*
    ============================================================
    WRITE TO FILE
    ============================================================
    */

    int
    pdf_stream_emit(
        pdf_stream_t *stream,
        FILE *file);

#ifdef __cplusplus
}
#endif

#endif
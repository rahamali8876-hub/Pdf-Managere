// core/pdf_content_stream.h


#ifndef PDF_CORE_CONTENT_STREAM_H
#define PDF_CORE_CONTENT_STREAM_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>

    typedef struct pdf_content_stream_s pdf_content_stream_t;

    /*
    ============================================================
    CREATE / DESTROY
    ============================================================
    */

    pdf_content_stream_t *
    pdf_content_stream_create(void);

    void
    pdf_content_stream_destroy(
        pdf_content_stream_t *stream);

    /*
    ============================================================
    DRAW IMAGE COMMAND
    ============================================================
    */

    int
    pdf_content_stream_draw_image(
        pdf_content_stream_t *stream,
        const char *image_name,
        float width,
        float height);

    /*
    ============================================================
    ACCESS
    ============================================================
    */

    const unsigned char *
    pdf_content_stream_data(
        const pdf_content_stream_t *stream);

    size_t
    pdf_content_stream_size(
        const pdf_content_stream_t *stream);

#ifdef __cplusplus
}
#endif

#endif
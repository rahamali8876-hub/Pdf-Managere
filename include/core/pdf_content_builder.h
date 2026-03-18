#ifndef PDF_CORE_CONTENT_BUILDER_H
#define PDF_CORE_CONTENT_BUILDER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stddef.h>

    typedef struct pdf_content_builder_s pdf_content_builder_t;

    /*
    ============================================================
    CREATE / DESTROY
    ============================================================
    */

    pdf_content_builder_t *
    pdf_content_builder_create(size_t initial_capacity);

    void
    pdf_content_builder_destroy(pdf_content_builder_t *cb);

    /*
    ============================================================
    RAW APPEND (LOW LEVEL)
    ============================================================
    */

    int
    pdf_cb_append(
        pdf_content_builder_t *cb,
        const char *fmt,
        ...);

    /*
    ============================================================
    GRAPHICS STATE
    ============================================================
    */

    int pdf_cb_save(pdf_content_builder_t *cb);    /* q */
    int pdf_cb_restore(pdf_content_builder_t *cb); /* Q */

    /*
    ============================================================
    TRANSFORM
    ============================================================
    */

    int
    pdf_cb_concat_matrix(
        pdf_content_builder_t *cb,
        float a, float b,
        float c, float d,
        float e, float f); /* cm */

    /*
    ============================================================
    DRAW IMAGE
    ============================================================
    */

    int
    pdf_cb_draw_image(
        pdf_content_builder_t *cb,
        const char *name,
        float x,
        float y,
        float w,
        float h);

    /*
    ============================================================
    OUTPUT
    ============================================================
    */

    const char *
    pdf_cb_data(const pdf_content_builder_t *cb);

    size_t
    pdf_cb_size(const pdf_content_builder_t *cb);

    int
    pdf_cb_write_stream(
        pdf_content_builder_t *cb,
        FILE *file,
        int object_number);

#ifdef __cplusplus
}
#endif

#endif
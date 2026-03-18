#ifndef PDF_CORE_IMAGE_H
#define PDF_CORE_IMAGE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>

    typedef struct pdf_image_s pdf_image_t;

    /*
    ============================================================
    CREATE / DESTROY
    ============================================================
    */

    pdf_image_t *
    pdf_image_create(
        int width,
        int height);

    void
    pdf_image_destroy(
        pdf_image_t *img);

    /*
    ============================================================
    IMAGE DATA
    ============================================================
    */

    int
    pdf_image_set_data(
        pdf_image_t *img,
        const unsigned char *data,
        size_t size);

    int
    pdf_image_width(
        const pdf_image_t *img);

    int
    pdf_image_height(
        const pdf_image_t *img);

    size_t
    pdf_image_data_size(
        const pdf_image_t *img);

#ifdef __cplusplus
}
#endif

#endif
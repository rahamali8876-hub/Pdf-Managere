// core/pdf_image_xobject.h

// #ifndef PDF_CORE_IMAGE_XOBJECT_H
// #define PDF_CORE_IMAGE_XOBJECT_H

// #ifdef __cplusplus
// extern "C"
// {
// #endif

// #include <stddef.h>

//     typedef struct pdf_image_xobject_s pdf_image_xobject_t;

//     /*
//     ============================================================
//     CREATE / DESTROY
//     ============================================================
//     */

//     pdf_image_xobject_t *
//     pdf_image_xobject_create(
//         int width,
//         int height,
//         const unsigned char *data,
//         size_t size);

//     void
//     pdf_image_xobject_destroy(
//         pdf_image_xobject_t *img);

//     /*
//     ============================================================
//     ACCESS
//     ============================================================
//     */

//     int pdf_image_xobject_width(
//         const pdf_image_xobject_t *img);

//     int pdf_image_xobject_height(
//         const pdf_image_xobject_t *img);

//     size_t pdf_image_xobject_size(
//         const pdf_image_xobject_t *img);

//     const unsigned char *
//     pdf_image_xobject_data(
//         const pdf_image_xobject_t *img);

// #ifdef __cplusplus
// }
// #endif

// #endif

#ifndef PDF_CORE_IMAGE_XOBJECT_H
#define PDF_CORE_IMAGE_XOBJECT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>

    typedef struct pdf_image_xobject_s pdf_image_xobject_t;

    /*
    ============================================================
    CREATE
    ============================================================
    */

    pdf_image_xobject_t *
    pdf_image_xobject_create(
        int width,
        int height,
        const unsigned char *data,
        size_t size);

    /* NEW: create with compression filter (JPEG etc) */

    pdf_image_xobject_t *
    pdf_image_xobject_create_with_filter(
        int width,
        int height,
        const unsigned char *data,
        size_t size,
        const char *filter);

    /*
    ============================================================
    DESTROY
    ============================================================
    */

    void
    pdf_image_xobject_destroy(
        pdf_image_xobject_t *img);

    /*
    ============================================================
    ACCESS
    ============================================================
    */

    int
    pdf_image_xobject_width(
        const pdf_image_xobject_t *img);

    int
    pdf_image_xobject_height(
        const pdf_image_xobject_t *img);

    size_t
    pdf_image_xobject_size(
        const pdf_image_xobject_t *img);

    const unsigned char *
    pdf_image_xobject_data(
        const pdf_image_xobject_t *img);

    /* NEW */

    const char *
    pdf_image_xobject_filter(
        const pdf_image_xobject_t *img);

#ifdef __cplusplus
}
#endif

#endif
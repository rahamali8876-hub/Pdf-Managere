// src/core/pdf_image_xobject.c

// #include "core/pdf_image_xobject.h"

// #include <stdlib.h>
// #include <string.h>

// struct pdf_image_xobject_s
// {
//     int width;
//     int height;

//     unsigned char *data;
//     size_t size;
// };

// /*
// ============================================================
// CREATE
// ============================================================
// */

// pdf_image_xobject_t *
// pdf_image_xobject_create(
//     int width,
//     int height,
//     const unsigned char *data,
//     size_t size)
// {
//     if (!data)
//         return NULL;

//     pdf_image_xobject_t *img =
//         malloc(sizeof(pdf_image_xobject_t));

//     if (!img)
//         return NULL;

//     img->width = width;
//     img->height = height;

//     img->data = malloc(size);

//     memcpy(img->data, data, size);

//     img->size = size;

//     return img;
// }

// /*
// ============================================================
// DESTROY
// ============================================================
// */

// void pdf_image_xobject_destroy(
//     pdf_image_xobject_t *img)
// {
//     if (!img)
//         return;

//     free(img->data);
//     free(img);
// }

// /*
// ============================================================
// ACCESS
// ============================================================
// */

// int pdf_image_xobject_width(
//     const pdf_image_xobject_t *img)
// {
//     return img ? img->width : 0;
// }

// int pdf_image_xobject_height(
//     const pdf_image_xobject_t *img)
// {
//     return img ? img->height : 0;
// }

// size_t
// pdf_image_xobject_size(
//     const pdf_image_xobject_t *img)
// {
//     return img ? img->size : 0;
// }

// const unsigned char *
// pdf_image_xobject_data(
//     const pdf_image_xobject_t *img)
// {
//     return img ? img->data : NULL;
// }

#include "core/pdf_image_xobject.h"

#include <stdlib.h>
#include <string.h>

struct pdf_image_xobject_s
{
    int width;
    int height;

    unsigned char *data;
    size_t size;

    const char *filter; /* JPEG = /DCTDecode */
};

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
    size_t size)
{
    if (!data)
        return NULL;

    pdf_image_xobject_t *img =
        malloc(sizeof(pdf_image_xobject_t));

    if (!img)
        return NULL;

    img->width = width;
    img->height = height;

    img->data = malloc(size);
    memcpy(img->data, data, size);

    img->size = size;
    img->filter = NULL;

    return img;
}

/*
============================================================
CREATE WITH FILTER
============================================================
*/

pdf_image_xobject_t *
pdf_image_xobject_create_with_filter(
    int width,
    int height,
    const unsigned char *data,
    size_t size,
    const char *filter)
{
    pdf_image_xobject_t *img =
        pdf_image_xobject_create(width, height, data, size);

    if (!img)
        return NULL;

    img->filter = filter;

    return img;
}

/*
============================================================
DESTROY
============================================================
*/

void pdf_image_xobject_destroy(
    pdf_image_xobject_t *img)
{
    if (!img)
        return;

    free(img->data);
    free(img);
}

/*
============================================================
ACCESS
============================================================
*/

int pdf_image_xobject_width(
    const pdf_image_xobject_t *img)
{
    return img ? img->width : 0;
}

int pdf_image_xobject_height(
    const pdf_image_xobject_t *img)
{
    return img ? img->height : 0;
}

size_t
pdf_image_xobject_size(
    const pdf_image_xobject_t *img)
{
    return img ? img->size : 0;
}

const unsigned char *
pdf_image_xobject_data(
    const pdf_image_xobject_t *img)
{
    return img ? img->data : NULL;
}

/*
============================================================
FILTER
============================================================
*/

const char *
pdf_image_xobject_filter(
    const pdf_image_xobject_t *img)
{
    return img ? img->filter : NULL;
}
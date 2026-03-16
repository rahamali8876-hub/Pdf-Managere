#include "core/pdf_image.h"

#include <stdlib.h>
#include <string.h>

struct pdf_image_s
{
    int width;
    int height;

    unsigned char *data;
    size_t size;
};

/*
============================================================
CREATE
============================================================
*/

pdf_image_t *
pdf_image_create(
    int width,
    int height)
{
    if (width <= 0 || height <= 0)
        return NULL;

    pdf_image_t *img =
        malloc(sizeof(pdf_image_t));

    if (!img)
        return NULL;

    img->width = width;
    img->height = height;

    img->data = NULL;
    img->size = 0;

    return img;
}

/*
============================================================
DESTROY
============================================================
*/

void pdf_image_destroy(
    pdf_image_t *img)
{
    if (!img)
        return;

    free(img->data);

    free(img);
}

/*
============================================================
SET DATA
============================================================
*/

int pdf_image_set_data(
    pdf_image_t *img,
    const unsigned char *data,
    size_t size)
{
    if (!img || !data)
        return -1;

    img->data =
        malloc(size);

    if (!img->data)
        return -1;

    memcpy(img->data, data, size);

    img->size = size;

    return 0;
}

/*
============================================================
ACCESSORS
============================================================
*/

int pdf_image_width(
    const pdf_image_t *img)
{
    return img ? img->width : 0;
}

int pdf_image_height(
    const pdf_image_t *img)
{
    return img ? img->height : 0;
}

size_t
pdf_image_data_size(
    const pdf_image_t *img)
{
    return img ? img->size : 0;
}
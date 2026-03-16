// src/image_loader/bmp_loader.c

#include "image_loader/jpeg_loader.h"
#include "image_loader/image_loader_registry.h"
#include "core/pdf_image_xobject.h"

#include <stdio.h>
#include <stdlib.h>

static pdf_image_xobject_t *
jpeg_load(const char *filename)
{
    FILE *f = fopen(filename, "rb");
    if (!f)
        return NULL;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    unsigned char *data =
        malloc(size);

    fread(data, 1, size, f);
    fclose(f);

    /* very simple JPEG dimension parser */

    int width = 0;
    int height = 0;

    for (long i = 0; i < size - 9; i++)
    {
        if (data[i] == 0xFF &&
            data[i + 1] == 0xC0)
        {
            height =
                (data[i + 5] << 8) | data[i + 6];

            width =
                (data[i + 7] << 8) | data[i + 8];

            break;
        }
    }

    return pdf_image_xobject_create_with_filter(
        width,
        height,
        data,
        size,
        "/DCTDecode");
}

void jpeg_loader_register(void)
{
    static image_loader_t jpeg_loader =
        {
            ".jpg",
            jpeg_load};

    image_loader_register(&jpeg_loader);
}
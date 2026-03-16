// src/image_loader/bmp_loader.c

#include "image_loader/bmp_loader.h"
#include "image_loader/image_loader_registry.h"

#include "core/pdf_image_xobject.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
============================================================
BMP FILE STRUCTURES
============================================================
*/

#pragma pack(push, 1)

typedef struct
{
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;

} bmp_file_header_t;

typedef struct
{
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bit_count;
    uint32_t compression;
    uint32_t image_size;
    int32_t x_pixels_per_meter;
    int32_t y_pixels_per_meter;
    uint32_t colors_used;
    uint32_t colors_important;

} bmp_info_header_t;

#pragma pack(pop)

/*
============================================================
READ BMP
============================================================
*/

static pdf_image_xobject_t *
bmp_load(const char *filename)
{
    FILE *f = fopen(filename, "rb");

    if (!f)
    {
        printf("BMP loader: cannot open file\n");
        return NULL;
    }

    bmp_file_header_t file_header;
    bmp_info_header_t info_header;

    fread(&file_header, sizeof(file_header), 1, f);

    if (file_header.type != 0x4D42) /* BM */
    {
        printf("Not a BMP file\n");
        fclose(f);
        return NULL;
    }

    fread(&info_header, sizeof(info_header), 1, f);

    int width = info_header.width;
    int height = info_header.height;
    int bpp = info_header.bit_count;

    if (bpp != 24)
    {
        printf("Only 24-bit BMP supported\n");
        fclose(f);
        return NULL;
    }

    printf("BMP: %dx%d\n", width, height);

    fseek(f, file_header.offset, SEEK_SET);

    int row_padded =
        (width * 3 + 3) & (~3);

    uint8_t *bmp_row =
        malloc(row_padded);

    uint8_t *rgb_data =
        malloc(width * height * 3);

    if (!bmp_row || !rgb_data)
    {
        fclose(f);
        return NULL;
    }

    /*
    ============================================================
    READ PIXELS (BMP stored bottom-up)
    ============================================================
    */

    for (int y = height - 1; y >= 0; y--)
    {
        fread(bmp_row, 1, row_padded, f);

        for (int x = 0; x < width; x++)
        {
            uint8_t b = bmp_row[x * 3 + 0];
            uint8_t g = bmp_row[x * 3 + 1];
            uint8_t r = bmp_row[x * 3 + 2];

            int index =
                (y * width + x) * 3;

            rgb_data[index + 0] = r;
            rgb_data[index + 1] = g;
            rgb_data[index + 2] = b;
        }
    }

    free(bmp_row);

    fclose(f);

    /*
    ============================================================
    CREATE PDF IMAGE XOBJECT
    ============================================================
    */

    size_t data_size = width * height * 3;

    pdf_image_xobject_t *img =
        pdf_image_xobject_create(
            width,
            height,
            rgb_data,
            data_size);

    free(rgb_data);

    return img;
}

/*
============================================================
LOADER REGISTRATION
============================================================
*/

static image_loader_t bmp_loader = {".bmp", bmp_load};

void bmp_loader_register()
{
    image_loader_register(&bmp_loader);
}
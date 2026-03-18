// tests/test_image_loader.c

#include "image_loader/image_loader_registry.h"
#include "image_loader/bmp_loader.h"

#include "core/pdf_content_stream.h"
#include "core/pdf_writer_document.h"

#include <stdio.h>

int main()
{
    printf("Initializing image loaders...\n");

    /* register loaders */
    bmp_loader_register();

    printf("Loading image...\n");

    pdf_image_xobject_t *img =
        image_loader_load("test.bmp");

    if (!img)
    {
        printf("Failed to load image\n");
        return 1;
    }

    printf("Creating content stream...\n");

    pdf_content_stream_t *content =
        pdf_content_stream_create();

    // pdf_content_stream_draw_image(
    //     content,
    //     "Im0",
    //     595,
    //     842);

    pdf_content_stream_draw_image(
        content,
        "Im0",
        pdf_image_xobject_width(img),
        pdf_image_xobject_height(img));

    printf("Writing PDF...\n");

    pdf_writer_write_image_page(
        "output.pdf",
        img,
        content);

    printf("PDF created: output.pdf\n");

    return 0;
}
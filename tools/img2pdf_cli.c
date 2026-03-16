// // // tools/img2pdf_cli.c

// #include "image_loader/image_loader.h"

// #include "core/pdf_document.h"
// #include "core/pdf_page.h"

// #include <stdio.h>

// #include "image_loader/image_loader_registry.h"

// int main(int argc, char **argv)
// {
//     image_loader_registry_init();

//     pdf_image_xobject_t *img =
//         image_loader_load(input);
//     // }

//     // int main(int argc, char **argv)
//     // {
//     if (argc < 3)
//     {
//         printf("Usage: img2pdf image1 image2 ... output.pdf\n");
//         return 1;
//     }

//     const char *output = argv[argc - 1];

//     printf("Creating document...\n");

//     pdf_document_t *doc =
//         pdf_document_create();

//     if (!doc)
//     {
//         printf("Failed to create document\n");
//         return 1;
//     }

//     /*
//     ------------------------------------------------
//     LOAD EACH IMAGE
//     ------------------------------------------------
//     */

//     for (int i = 1; i < argc - 1; i++)
//     {
//         const char *input = argv[i];

//         printf("Loading %s...\n", input);

//         pdf_image_xobject_t *img =
//             pdf_image_load_from_file(input);

//         if (!img)
//         {
//             printf("Failed to load %s\n", input);
//             continue;
//         }

//         int w = pdf_image_xobject_width(img);
//         int h = pdf_image_xobject_height(img);

//         printf("Image size: %dx%d\n", w, h);

//         /*
//         Create page matching image size
//         */

//         pdf_page_t *page =
//             pdf_document_add_page(doc, w, h);

//         if (!page)
//         {
//             printf("Failed to create page\n");
//             continue;
//         }

//         /*
//         Future step:
//         attach image + content stream to page
//         */
//     }

//     printf("Saving PDF...\n");

//     if (pdf_document_save(doc, output) != 0)
//     {
//         printf("Failed to save PDF\n");
//         pdf_document_destroy(doc);
//         return 1;
//     }

//     pdf_document_destroy(doc);

//     printf("PDF created: %s\n", output);

//     return 0;
// }

#include "image_loader/image_loader_registry.h"

#include "core/pdf_document.h"
#include "core/pdf_page.h"

#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Usage: img2pdf image1 image2 ... output.pdf\n");
        return 1;
    }

    /*
    ------------------------------------------------
    INIT IMAGE LOADERS
    ------------------------------------------------
    */

    image_loader_registry_init();

    const char *output = argv[argc - 1];

    printf("Creating document...\n");

    pdf_document_t *doc = pdf_document_create();

    if (!doc)
    {
        printf("Failed to create document\n");
        return 1;
    }

    /*
    ------------------------------------------------
    LOAD IMAGES
    ------------------------------------------------
    */

    for (int i = 1; i < argc - 1; i++)
    {
        const char *input = argv[i];

        printf("Loading %s...\n", input);

        pdf_image_xobject_t *img =
            image_loader_load(input);

        if (!img)
        {
            printf("Failed to load %s\n", input);
            continue;
        }

        int w = pdf_image_xobject_width(img);
        int h = pdf_image_xobject_height(img);

        printf("Image size: %dx%d\n", w, h);

        /*
        CREATE PAGE
        */

        pdf_page_t *page =
            pdf_document_add_page(doc, w, h);

        if (!page)
        {
            printf("Failed to create page\n");
            continue;
        }

        /*
        Future step:
        attach image to page resources
        */
    }

    /*
    ------------------------------------------------
    SAVE DOCUMENT
    ------------------------------------------------
    */

    printf("Saving PDF...\n");

    if (pdf_document_save(doc, output) != 0)
    {
        printf("Failed to save PDF\n");
        pdf_document_destroy(doc);
        return 1;
    }

    pdf_document_destroy(doc);

    printf("PDF created: %s\n", output);

    return 0;
}
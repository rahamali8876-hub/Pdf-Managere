// // src/core/pdf_document.c

#include "core/pdf_document.h"

#include <stdlib.h>
#include <stdio.h>

#include "system/vector.h"

#include "core/pdf_object_manager.h"
#include "core/pdf_page_resources.h"
#include "core/pdf_content_builder.h"

/*
============================================================
PAGE STRUCTURE
============================================================
*/

typedef struct
{
    pdf_image_xobject_t *image;

} pdf_page_internal_t;

/*
============================================================
DOCUMENT
============================================================
*/

struct pdf_document_s
{
    cleaner_vector_t *pages;
};

/*
============================================================
CREATE
============================================================
*/

pdf_document_t *
pdf_document_create(void)
{
    pdf_document_t *doc =
        malloc(sizeof(pdf_document_t));

    if (!doc)
        return NULL;

    doc->pages =
        cleaner_vector_create(
            NULL,
            sizeof(pdf_page_internal_t),
            4);

    if (!doc->pages)
    {
        free(doc);
        return NULL;
    }

    return doc;
}

/*
============================================================
DESTROY
============================================================
*/

void pdf_document_destroy(pdf_document_t *doc)
{
    if (!doc)
        return;

    cleaner_vector_destroy(doc->pages);

    free(doc);
}

/*
============================================================
ADD IMAGE PAGE
============================================================
*/

int pdf_document_add_image_page(
    pdf_document_t *doc,
    pdf_image_xobject_t *image)
{
    if (!doc || !image)
        return -1;

    pdf_page_internal_t page;
    page.image = image;

    return cleaner_vector_push_back(
        doc->pages,
        &page);
}

/*
============================================================
SAVE DOCUMENT
============================================================
*/

int pdf_document_save(
    pdf_document_t *doc,
    const char *path)
{
    if (!doc || !path)
        return -1;

    FILE *f = fopen(path, "wb");
    if (!f)
        return -1;

    fprintf(f, "%%PDF-1.4\n");

    /*
    ============================================================
    INIT
    ============================================================
    */

    pdf_object_manager_t *mgr =
        pdf_object_manager_create(32);

    int catalog = pdf_object_manager_new(mgr);
    int pages = pdf_object_manager_new(mgr);

    size_t page_count =
        cleaner_vector_size(doc->pages);

    /*
    store object ids
    */

    int *page_ids =
        malloc(sizeof(int) * page_count);

    int *content_ids =
        malloc(sizeof(int) * page_count);

    int *image_ids =
        malloc(sizeof(int) * page_count);

    /*
    allocate objects
    */

    for (size_t i = 0; i < page_count; i++)
    {
        page_ids[i] = pdf_object_manager_new(mgr);
        image_ids[i] = pdf_object_manager_new(mgr);
        content_ids[i] = pdf_object_manager_new(mgr);
    }

    /*
    ============================================================
    1. CATALOG
    ============================================================
    */

    pdf_object_manager_begin(mgr, f, catalog);

    fprintf(f,
            "<< /Type /Catalog /Pages %d 0 R >>\n",
            pages);

    pdf_object_manager_end(f);

    /*
    ============================================================
    2. PAGES TREE
    ============================================================
    */

    pdf_object_manager_begin(mgr, f, pages);

    fprintf(f, "<< /Type /Pages /Kids [");

    for (size_t i = 0; i < page_count; i++)
    {
        fprintf(f, "%d 0 R ", page_ids[i]);
    }

    fprintf(f, "] /Count %zu >>\n", page_count);

    pdf_object_manager_end(f);

    /*
    ============================================================
    WRITE EACH PAGE
    ============================================================
    */

    for (size_t i = 0; i < page_count; i++)
    {
        pdf_page_internal_t *p =
            cleaner_vector_at(doc->pages, i);

        pdf_image_xobject_t *img = p->image;

        /*
        resources
        */

        pdf_page_resources_t *res =
            pdf_page_resources_create();

        const char *img_name =
            pdf_page_resources_add_image(
                res,
                image_ids[i]);

        /*
        content
        */

        pdf_content_builder_t *cb =
            pdf_content_builder_create(256);

        pdf_cb_draw_image(
            cb,
            img_name,
            0,
            0,
            (float)pdf_image_xobject_width(img),
            (float)pdf_image_xobject_height(img));

        /*
        -------------------------
        PAGE OBJECT
        -------------------------
        */

        pdf_object_manager_begin(mgr, f, page_ids[i]);

        fprintf(f,
                "<< /Type /Page\n"
                "/Parent %d 0 R\n"
                "/MediaBox [0 0 %d %d]\n"
                "/Resources ",
                pages,
                pdf_image_xobject_width(img),
                pdf_image_xobject_height(img));

        pdf_page_resources_write(res, f);

        fprintf(f,
                "/Contents %d 0 R\n"
                ">>\n",
                content_ids[i]);

        pdf_object_manager_end(f);

        /*
        -------------------------
        IMAGE OBJECT
        -------------------------
        */

        pdf_object_manager_begin(mgr, f, image_ids[i]);

        fprintf(f,
                "<< /Type /XObject\n"
                "/Subtype /Image\n"
                "/Width %d\n"
                "/Height %d\n"
                "/ColorSpace /DeviceRGB\n"
                "/BitsPerComponent 8\n",
                pdf_image_xobject_width(img),
                pdf_image_xobject_height(img));

        if (pdf_image_xobject_filter(img))
        {
            fprintf(f,
                    "/Filter %s\n",
                    pdf_image_xobject_filter(img));
        }

        fprintf(f,
                "/Length %zu >>\n"
                "stream\n",
                pdf_image_xobject_size(img));

        fwrite(
            pdf_image_xobject_data(img),
            1,
            pdf_image_xobject_size(img),
            f);

        fprintf(f, "\nendstream\n");

        pdf_object_manager_end(f);

        /*
        -------------------------
        CONTENT OBJECT
        -------------------------
        */

        pdf_object_manager_begin(mgr, f, content_ids[i]);

        fprintf(f,
                "<< /Length %zu >>\n"
                "stream\n",
                pdf_cb_size(cb));

        fwrite(
            pdf_cb_data(cb),
            1,
            pdf_cb_size(cb),
            f);

        fprintf(f, "\nendstream\n");

        pdf_object_manager_end(f);

        /*
        cleanup per-page
        */

        pdf_content_builder_destroy(cb);
        pdf_page_resources_destroy(res);
    }

    /*
    ============================================================
    FINALIZE
    ============================================================
    */

    pdf_object_manager_write_xref(
        mgr,
        f,
        catalog);

    /*
    cleanup
    */

    free(page_ids);
    free(image_ids);
    free(content_ids);

    pdf_object_manager_destroy(mgr);

    fclose(f);

    return 0;
}

// #include "core/pdf_document.h"
// #include "system/vector.h"

// #include <stdlib.h>
// #include <stdio.h>

// /*
// ============================================================
// DOCUMENT STRUCTURE
// ============================================================
// */

// struct pdf_document_s
// {
//     cleaner_vector_t *pages;
// };

// /*
// ============================================================
// CREATE
// ============================================================
// */

// pdf_document_t *
// pdf_document_create(void)
// {
//     pdf_document_t *doc =
//         (pdf_document_t *)malloc(sizeof(pdf_document_t));

//     if (!doc)
//         return NULL;

//     doc->pages =
//         cleaner_vector_create(NULL, sizeof(pdf_page_t *), 8);

//     if (!doc->pages)
//     {
//         free(doc);
//         return NULL;
//     }

//     return doc;
// }

// /*
// ============================================================
// DESTROY
// ============================================================
// */

// void pdf_document_destroy(pdf_document_t *doc)
// {
//     if (!doc)
//         return;

//     size_t count =
//         cleaner_vector_size(doc->pages);

//     for (size_t i = 0; i < count; i++)
//     {
//         pdf_page_t **p =
//             cleaner_vector_at(doc->pages, i);

//         if (p && *p)
//             pdf_page_destroy(*p);
//     }

//     cleaner_vector_destroy(doc->pages);

//     free(doc);
// }

// /*
// ============================================================
// ADD PAGE
// ============================================================
// */

// pdf_page_t *
// pdf_document_add_page(
//     pdf_document_t *doc,
//     int width,
//     int height)
// {
//     if (!doc)
//         return NULL;

//     pdf_page_t *page =
//         pdf_page_create(width, height);

//     if (!page)
//         return NULL;

//     cleaner_vector_push_back(
//         doc->pages,
//         &page);

//     return page;
// }

// /*
// ============================================================
// PAGE COUNT
// ============================================================
// */

// size_t
// pdf_document_page_count(
//     const pdf_document_t *doc)
// {
//     if (!doc)
//         return 0;

//     return cleaner_vector_size(doc->pages);
// }

// /*
// ============================================================
// GET PAGE
// ============================================================
// */

// pdf_page_t *
// pdf_document_get_page(
//     pdf_document_t *doc,
//     size_t index)
// {
//     if (!doc)
//         return NULL;

//     pdf_page_t **page =
//         cleaner_vector_at(doc->pages, index);

//     if (!page)
//         return NULL;

//     return *page;
// }

// /*
// ============================================================
// SAVE (MINIMAL PDF WRITER)
// ============================================================
// */

// int pdf_document_save(
//     pdf_document_t *doc,
//     const char *path)
// {
//     if (!doc || !path)
//         return -1;

//     FILE *f = fopen(path, "wb");
//     if (!f)
//         return -1;

//     size_t page_count =
//         pdf_document_page_count(doc);

//     if (page_count == 0)
//     {
//         fclose(f);
//         return -1;
//     }

//     long *offsets =
//         malloc(sizeof(long) * (page_count + 3));

//     fprintf(f, "%%PDF-1.4\n");

//     /*
//     ----------------------------------------
//     1 Catalog
//     ----------------------------------------
//     */

//     offsets[1] = ftell(f);

//     fprintf(f,
//             "1 0 obj\n"
//             "<< /Type /Catalog /Pages 2 0 R >>\n"
//             "endobj\n");

//     /*
//     ----------------------------------------
//     2 Pages Tree
//     ----------------------------------------
//     */

//     offsets[2] = ftell(f);

//     fprintf(f,
//             "2 0 obj\n"
//             "<< /Type /Pages\n"
//             "/Count %zu\n"
//             "/Kids [",
//             page_count);

//     for (size_t i = 0; i < page_count; i++)
//     {
//         fprintf(f, "%zu 0 R ", i + 3);
//     }

//     fprintf(f,
//             "] >>\n"
//             "endobj\n");

//     /*
//     ----------------------------------------
//     PAGE OBJECTS
//     ----------------------------------------
//     */

//     for (size_t i = 0; i < page_count; i++)
//     {
//         pdf_page_t *page =
//             pdf_document_get_page(doc, i);

//         offsets[i + 3] = ftell(f);

//         fprintf(f,
//                 "%zu 0 obj\n"
//                 "<< /Type /Page\n"
//                 "/Parent 2 0 R\n"
//                 "/MediaBox [0 0 %d %d]\n"
//                 ">>\n"
//                 "endobj\n",
//                 i + 3,
//                 pdf_page_width(page),
//                 pdf_page_height(page));
//     }

//     /*
//     ----------------------------------------
//     XREF
//     ----------------------------------------
//     */

//     long xref_pos = ftell(f);

//     fprintf(f, "xref\n");
//     fprintf(f, "0 %zu\n", page_count + 3);

//     fprintf(f, "0000000000 65535 f \n");

//     for (size_t i = 1; i < page_count + 3; i++)
//     {
//         fprintf(f,
//                 "%010ld 00000 n \n",
//                 offsets[i]);
//     }

//     /*
//     ----------------------------------------
//     TRAILER
//     ----------------------------------------
//     */

//     fprintf(f,
//             "trailer\n"
//             "<< /Size %zu\n"
//             "/Root 1 0 R >>\n",
//             page_count + 3);

//     fprintf(f,
//             "startxref\n"
//             "%ld\n"
//             "%%%%EOF",
//             xref_pos);

//     free(offsets);

//     fclose(f);

//     return 0;
// }
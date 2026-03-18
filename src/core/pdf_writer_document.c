// src/core/pdf_writer_document.c

#include "core/pdf_writer_document.h"

#include <stdio.h>

#include "core/pdf_object_manager.h"
#include "core/pdf_page_resources.h"
#include "core/pdf_content_builder.h"

/*
============================================================
FINAL IMAGE → PDF WRITER
============================================================
*/

int pdf_writer_write_image_page(
    const char *path,
    pdf_image_xobject_t *image,
    pdf_content_stream_t *content_unused) /* legacy */
{
        FILE *f = fopen(path, "wb");
        if (!f)
                return -1;

        fprintf(f, "%%PDF-1.4\n");

        /*
        ============================================================
        INIT SYSTEMS
        ============================================================
        */

        pdf_object_manager_t *mgr =
            pdf_object_manager_create(16);

        pdf_page_resources_t *res =
            pdf_page_resources_create();

        pdf_content_builder_t *cb =
            pdf_content_builder_create(256);

        if (!mgr || !res || !cb)
                return -1;

        /*
        ============================================================
        CREATE OBJECT IDS
        ============================================================
        */

        int catalog = pdf_object_manager_new(mgr);
        int pages = pdf_object_manager_new(mgr);
        int page = pdf_object_manager_new(mgr);
        int image_obj = pdf_object_manager_new(mgr);
        int content_obj = pdf_object_manager_new(mgr);

        /*
        ============================================================
        REGISTER IMAGE RESOURCE
        ============================================================
        */

        const char *img_name =
            pdf_page_resources_add_image(
                res,
                image_obj);

        /*
        ============================================================
        BUILD CONTENT STREAM (DSL)
        ============================================================
        */

        pdf_cb_draw_image(
            cb,
            img_name,
            0,
            0,
            (float)pdf_image_xobject_width(image),
            (float)pdf_image_xobject_height(image));

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
        2. PAGES
        ============================================================
        */

        pdf_object_manager_begin(mgr, f, pages);

        fprintf(f,
                "<< /Type /Pages /Kids [%d 0 R] /Count 1 >>\n",
                page);

        pdf_object_manager_end(f);

        /*
        ============================================================
        3. PAGE
        ============================================================
        */

        pdf_object_manager_begin(mgr, f, page);

        fprintf(f,
                "<< /Type /Page\n"
                "/Parent %d 0 R\n"
                "/MediaBox [0 0 %d %d]\n"
                "/Resources ",
                pages,
                pdf_image_xobject_width(image),
                pdf_image_xobject_height(image));

        pdf_page_resources_write(res, f);

        fprintf(f,
                "/Contents %d 0 R\n"
                ">>\n",
                content_obj);

        pdf_object_manager_end(f);

        /*
        ============================================================
        4. IMAGE XOBJECT
        ============================================================
        */

        pdf_object_manager_begin(mgr, f, image_obj);

        fprintf(f,
                "<< /Type /XObject\n"
                "/Subtype /Image\n"
                "/Width %d\n"
                "/Height %d\n"
                "/ColorSpace /DeviceRGB\n"
                "/BitsPerComponent 8\n",
                pdf_image_xobject_width(image),
                pdf_image_xobject_height(image));

        if (pdf_image_xobject_filter(image))
        {
                fprintf(f,
                        "/Filter %s\n",
                        pdf_image_xobject_filter(image));
        }

        fprintf(f,
                "/Length %zu >>\n"
                "stream\n",
                pdf_image_xobject_size(image));

        fwrite(
            pdf_image_xobject_data(image),
            1,
            pdf_image_xobject_size(image),
            f);

        fprintf(f,
                "\nendstream\n");

        pdf_object_manager_end(f);

        /*
        ============================================================
        5. CONTENT STREAM (FROM BUILDER)
        ============================================================
        */

        pdf_object_manager_begin(mgr, f, content_obj);

        fprintf(f,
                "<< /Length %zu >>\n"
                "stream\n",
                pdf_cb_size(cb));

        fwrite(
            pdf_cb_data(cb),
            1,
            pdf_cb_size(cb),
            f);

        fprintf(f,
                "\nendstream\n");

        pdf_object_manager_end(f);

        /*
        ============================================================
        FINALIZE FILE
        ============================================================
        */

        pdf_object_manager_write_xref(
            mgr,
            f,
            catalog);

        /*
        ============================================================
        CLEANUP
        ============================================================
        */

        pdf_content_builder_destroy(cb);
        pdf_page_resources_destroy(res);
        pdf_object_manager_destroy(mgr);

        fclose(f);

        return 0;
}
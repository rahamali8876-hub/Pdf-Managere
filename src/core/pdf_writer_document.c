// // // src/core/pdf_writer_document.c

#include "core/pdf_writer_document.h"

#include <stdio.h>

#include "core/pdf_object_manager.h"

FILE *f = fopen("output.pdf", "wb");

fprintf(f, "%%PDF-1.4\n");

pdf_object_manager_t *mgr =
    pdf_object_manager_create(16);

pdf_object_id catalog =
    pdf_object_manager_new(mgr);

pdf_object_id pages =
    pdf_object_manager_new(mgr);

/* catalog */

pdf_object_manager_begin(mgr, f, catalog);

fprintf(f,
        "<< /Type /Catalog /Pages %d 0 R >>\n",
        pages);

pdf_object_manager_end(f);

/* pages */

pdf_object_manager_begin(mgr, f, pages);

fprintf(f,
        "<< /Type /Pages /Count 0 >>\n");

pdf_object_manager_end(f);

/* write xref */

pdf_xref_t *xref =
    pdf_xref_create(16);

/* object 1 */

pdf_xref_register(xref, 1, ftell(f));
fprintf(f, "1 0 obj\n");
fprintf(f, "<< /Type /Catalog /Pages 2 0 R >>\n");
fprintf(f, "endobj\n");

/* object 2 */

pdf_xref_register(xref, 2, ftell(f));
fprintf(f, "2 0 obj\n");
fprintf(f, "<< /Type /Pages /Count 0 >>\n");
fprintf(f, "endobj\n");

/* write xref */

long pos = ftell(f);

pdf_xref_write(xref, f, pos);

pdf_xref_destroy(xref);

pdf_object_manager_write_xref(
    mgr,
    f,
    catalog);

pdf_object_manager_destroy(mgr);

fclose(f);

int pdf_writer_write_image_page(
    const char *path,
    pdf_image_xobject_t *image,
    pdf_content_stream_t *content)
{
        FILE *f = fopen(path, "wb");
        if (!f)
                return -1;

        /* object offsets for xref */
        long offsets[6];

        fprintf(f, "%%PDF-1.4\n");

        /* --------------------------------------------------
           1 Catalog
        -------------------------------------------------- */

        offsets[1] = ftell(f);
        fprintf(f,
                "1 0 obj\n"
                "<< /Type /Catalog /Pages 2 0 R >>\n"
                "endobj\n");

        /* --------------------------------------------------
           2 Pages
        -------------------------------------------------- */

        offsets[2] = ftell(f);
        fprintf(f,
                "2 0 obj\n"
                "<< /Type /Pages /Kids [3 0 R] /Count 1 >>\n"
                "endobj\n");

        /* --------------------------------------------------
           3 Page
        -------------------------------------------------- */

        offsets[3] = ftell(f);
        fprintf(f,
                "3 0 obj\n"
                "<< /Type /Page\n"
                "/Parent 2 0 R\n"
                "/Resources << /XObject << /Im0 4 0 R >> >>\n"
                "/MediaBox [0 0 %d %d]\n"
                "/Contents 5 0 R >>\n"
                "endobj\n",
                pdf_image_xobject_width(image),
                pdf_image_xobject_height(image));

        /* --------------------------------------------------
           4 Image XObject
        -------------------------------------------------- */

        offsets[4] = ftell(f);

        fprintf(f,
                "4 0 obj\n"
                "<< /Type /XObject\n"
                "/Subtype /Image\n"
                "/Width %d\n"
                "/Height %d\n"
                "/ColorSpace /DeviceRGB\n"
                "/BitsPerComponent 8\n",
                pdf_image_xobject_width(image),
                pdf_image_xobject_height(image));

        /* optional compression filter (JPEG) */

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
                "\nendstream\n"
                "endobj\n");

        /* --------------------------------------------------
           5 Content Stream
        -------------------------------------------------- */

        offsets[5] = ftell(f);

        fprintf(f,
                "5 0 obj\n"
                "<< /Length %zu >>\n"
                "stream\n",
                pdf_content_stream_size(content));

        fwrite(
            pdf_content_stream_data(content),
            1,
            pdf_content_stream_size(content),
            f);

        fprintf(f,
                "\nendstream\n"
                "endobj\n");

        /* --------------------------------------------------
           XREF TABLE
        -------------------------------------------------- */

        long xref_pos = ftell(f);

        fprintf(f, "xref\n");
        fprintf(f, "0 6\n");

        /* object 0 (required) */

        fprintf(f,
                "0000000000 65535 f \n");

        /* real objects */

        for (int i = 1; i <= 5; i++)
        {
                fprintf(f,
                        "%010ld 00000 n \n",
                        offsets[i]);
        }

        /* --------------------------------------------------
           TRAILER
        -------------------------------------------------- */

        fprintf(f,
                "trailer\n"
                "<< /Size 6\n"
                "/Root 1 0 R >>\n");

        fprintf(f,
                "startxref\n"
                "%ld\n"
                "%%%%EOF",
                xref_pos);

        fclose(f);

        return 0;
}
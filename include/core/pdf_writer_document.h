// core/pdf_writer_document.h


#ifndef PDF_CORE_WRITER_DOCUMENT_H
#define PDF_CORE_WRITER_DOCUMENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "core/pdf_content_stream.h"
#include "core/pdf_image_xobject.h"

    int
    pdf_writer_write_image_page(
        const char *path,
        pdf_image_xobject_t *image,
        pdf_content_stream_t *content);

#ifdef __cplusplus
}
#endif

#endif
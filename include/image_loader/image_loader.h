// image_loader/image_loader.h

#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "core/pdf_image_xobject.h"

    typedef struct image_loader_s
    {
        const char *extension;

        pdf_image_xobject_t *(*load)(
            const char *filename);

    } image_loader_t;

#ifdef __cplusplus
}
#endif

#endif
// image_loader/image_loader_registry.h

#ifndef IMAGE_LOADER_REGISTRY_H
#define IMAGE_LOADER_REGISTRY_H

#include "image_loader/image_loader.h"

void image_loader_registry_init();

void image_loader_register(
    image_loader_t *loader);

pdf_image_xobject_t *
image_loader_load(
    const char *filename);

#endif
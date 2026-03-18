// src/image_loader/image_loader_registry.c

#include "image_loader/image_loader_registry.h"

/* loaders */

extern image_loader_t bmp_loader;
extern image_loader_t jpeg_loader;

void image_loader_registry_init()
{
    image_loader_register(&bmp_loader);
    image_loader_register(&jpeg_loader);
}

/*
============================================================
Registering IMAGE
============================================================
*/

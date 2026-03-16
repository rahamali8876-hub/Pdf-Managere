// // src/image_loader/image_loader_registry.c

// #include "image_loader/image_loader_registry.h"

// #include <string.h>

// #define MAX_LOADERS 16

// static image_loader_t *loaders[MAX_LOADERS];
// static int loader_count = 0;

// void image_loader_register(
//     image_loader_t *loader)
// {
//     if (loader_count < MAX_LOADERS)
//         loaders[loader_count++] = loader;
// }

// pdf_image_xobject_t *
// image_loader_load(
//     const char *filename)
// {
//     const char *ext =
//         strrchr(filename, '.');

//     if (!ext)
//         return NULL;

//     for (int i = 0; i < loader_count; i++)
//     {
//         if (strcmp(ext, loaders[i]->extension) == 0)
//         {
//             return loaders[i]->load(filename);
//         }
//     }

//     return NULL;
// }


#include "image_loader/image_loader_registry.h"

#include <string.h>
#include <ctype.h>

#define MAX_LOADERS 16

static image_loader_t *loaders[MAX_LOADERS];
static int loader_count = 0;

/*
============================================================
REGISTER
============================================================
*/

void image_loader_register(
    image_loader_t *loader)
{
    if (!loader)
        return;

    if (loader_count < MAX_LOADERS)
        loaders[loader_count++] = loader;
}

/*
============================================================
LOWERCASE EXTENSION
============================================================
*/

static void normalize_ext(char *dst, const char *src)
{
    while (*src)
    {
        *dst++ = (char)tolower(*src++);
    }

    *dst = 0;
}

/*
============================================================
LOAD IMAGE
============================================================
*/

pdf_image_xobject_t *
image_loader_load(
    const char *filename)
{
    const char *ext =
        strrchr(filename, '.');

    if (!ext)
        return NULL;

    char normalized[16];

    normalize_ext(normalized, ext);

    for (int i = 0; i < loader_count; i++)
    {
        if (strcmp(normalized, loaders[i]->extension) == 0)
        {
            return loaders[i]->load(filename);
        }
    }

    return NULL;
}
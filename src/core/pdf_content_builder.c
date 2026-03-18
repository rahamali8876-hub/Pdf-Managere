#include "core/pdf_content_builder.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/*
============================================================
STRUCTURE
============================================================
*/

struct pdf_content_builder_s
{
    char *buffer;
    size_t size;
    size_t capacity;
};

/*
============================================================
INTERNAL GROW
============================================================
*/

static int
pdf_cb_grow(pdf_content_builder_t *cb, size_t extra)
{
    if (cb->size + extra < cb->capacity)
        return 0;

    size_t new_cap = cb->capacity * 2 + extra;

    char *new_buf =
        (char *)realloc(cb->buffer, new_cap);

    if (!new_buf)
        return -1;

    cb->buffer = new_buf;
    cb->capacity = new_cap;

    return 0;
}

/*
============================================================
CREATE
============================================================
*/

pdf_content_builder_t *
pdf_content_builder_create(size_t initial_capacity)
{
    pdf_content_builder_t *cb =
        (pdf_content_builder_t *)malloc(
            sizeof(pdf_content_builder_t));

    if (!cb)
        return NULL;

    cb->buffer =
        (char *)malloc(initial_capacity);

    if (!cb->buffer)
    {
        free(cb);
        return NULL;
    }

    cb->size = 0;
    cb->capacity = initial_capacity;

    return cb;
}

/*
============================================================
DESTROY
============================================================
*/

void pdf_content_builder_destroy(pdf_content_builder_t *cb)
{
    if (!cb)
        return;

    free(cb->buffer);
    free(cb);
}

/*
============================================================
APPEND FORMATTED
============================================================
*/

int pdf_cb_append(
    pdf_content_builder_t *cb,
    const char *fmt,
    ...)
{
    if (!cb || !fmt)
        return -1;

    va_list args;
    va_start(args, fmt);

    char temp[512];

    int written =
        vsnprintf(temp, sizeof(temp), fmt, args);

    va_end(args);

    if (written <= 0)
        return -1;

    if (pdf_cb_grow(cb, (size_t)written + 1) != 0)
        return -1;

    memcpy(cb->buffer + cb->size, temp, written);

    cb->size += written;

    return 0;
}

/*
============================================================
GRAPHICS STATE
============================================================
*/

int pdf_cb_save(pdf_content_builder_t *cb)
{
    return pdf_cb_append(cb, "q\n");
}

int pdf_cb_restore(pdf_content_builder_t *cb)
{
    return pdf_cb_append(cb, "Q\n");
}

/*
============================================================
TRANSFORM (cm)
============================================================
*/

int pdf_cb_concat_matrix(
    pdf_content_builder_t *cb,
    float a, float b,
    float c, float d,
    float e, float f)
{
    return pdf_cb_append(
        cb,
        "%.3f %.3f %.3f %.3f %.3f %.3f cm\n",
        a, b, c, d, e, f);
}

/*
============================================================
DRAW IMAGE
============================================================
*/

int pdf_cb_draw_image(
    pdf_content_builder_t *cb,
    const char *name,
    float x,
    float y,
    float w,
    float h)
{
    if (!cb || !name)
        return -1;

    pdf_cb_save(cb);

    pdf_cb_concat_matrix(cb,
                         w, 0,
                         0, h,
                         x, y);

    pdf_cb_append(cb,
                  "/%s Do\n",
                  name);

    pdf_cb_restore(cb);

    return 0;
}

/*
============================================================
ACCESSORS
============================================================
*/

const char *
pdf_cb_data(const pdf_content_builder_t *cb)
{
    return cb ? cb->buffer : NULL;
}

size_t
pdf_cb_size(const pdf_content_builder_t *cb)
{
    return cb ? cb->size : 0;
}

/*
============================================================
WRITE STREAM OBJECT
============================================================
*/

int pdf_cb_write_stream(
    pdf_content_builder_t *cb,
    FILE *file,
    int object_number)
{
    if (!cb || !file)
        return -1;

    fprintf(file, "%d 0 obj\n", object_number);

    fprintf(file,
            "<< /Length %zu >>\n",
            cb->size);

    fprintf(file, "stream\n");

    fwrite(cb->buffer, 1, cb->size, file);

    fprintf(file, "\nendstream\n");
    fprintf(file, "endobj\n");

    return 0;
}
// src/core/pdf_content_stream.c

#include "core/pdf_content_stream.h"
#include "system/vector.h"
#include "core/pdf_page_resources.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct pdf_content_stream_s
{
    cleaner_vector_t *buffer;
};

/*
============================================================
CREATE
============================================================
*/

pdf_content_stream_t *
pdf_content_stream_create(void)
{
    pdf_content_stream_t *s =
        malloc(sizeof(pdf_content_stream_t));

    if (!s)
        return NULL;

    s->buffer =
        cleaner_vector_create(
            NULL,
            sizeof(unsigned char),
            256);

    return s;
}

/*
============================================================
DESTROY
============================================================
*/

void pdf_content_stream_destroy(
    pdf_content_stream_t *stream)
{
    if (!stream)
        return;

    cleaner_vector_destroy(stream->buffer);

    free(stream);
}

/*
============================================================
WRITE TEXT
============================================================
*/

static void append_text(
    pdf_content_stream_t *s,
    const char *text)
{
    size_t len = strlen(text);

    for (size_t i = 0; i < len; i++)
    {
        cleaner_vector_push_back(
            s->buffer,
            &text[i]);
    }
}

/*
============================================================
DRAW IMAGE
============================================================
*/

int pdf_content_stream_draw_image(
    pdf_content_stream_t *stream,
    const char *image_name,
    float width,
    float height)
{
    if (!stream || !image_name)
        return -1;

    char buffer[256];

    append_text(stream, "q\n");

    snprintf(
        buffer,
        sizeof(buffer),
        "%.2f 0 0 %.2f 0 0 cm\n",
        width,
        height);

    append_text(stream, buffer);

    snprintf(
        buffer,
        sizeof(buffer),
        "/%s Do\n",
        image_name);

    append_text(stream, buffer);

    append_text(stream, "Q\n");

    return 0;
}

// int pdf_content_stream_draw_image(
//     pdf_content_stream_t *stream,
//     const char *image_name,
//     float width,
//     float height)
// {
//     if (!stream || !image_name)
//         return -1;

//     char buffer[256];

//     append_text(stream, "q\n");

//     snprintf(
//         buffer,
//         sizeof(buffer),
//         "%.2f 0 0 %.2f 0 0 cm\n",
//         width,
//         height);

//     fprintf(f,
//             "q\n"
//             "200 0 0 200 50 50 cm\n"
//             "/%s Do\n"
//             "Q\n",
//             image_name);

//     fprintf(f,
//             "<<\n"
//             "/Type /Page\n"
//             "/Parent 2 0 R\n"
//             "/Resources ");

//     pdf_page_resources_write(res, f);

//     fprintf(f,
//             "/Contents 7 0 R\n"
//             ">>\n");

//     append_text(stream, buffer);

//     snprintf(
//         buffer,
//         sizeof(buffer),
//         "/%s Do\n",
//         image_name);

//     append_text(stream, buffer);

//     append_text(stream, "Q\n");

//     return 0;
// }

/*
============================================================
ACCESS
============================================================
*/

const unsigned char *
pdf_content_stream_data(
    const pdf_content_stream_t *stream)
{
    return stream ? cleaner_vector_data(stream->buffer)
                  : NULL;
}

size_t
pdf_content_stream_size(
    const pdf_content_stream_t *stream)
{
    return stream ? cleaner_vector_size(stream->buffer)
                  : 0;
}
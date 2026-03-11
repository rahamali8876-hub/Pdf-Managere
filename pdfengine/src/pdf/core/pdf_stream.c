#include "pdf/core/pdf_stream.h"
#include "system/vector.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct pdf_stream_s
{
    cleaner_vector_t *buffer;
};

/*
============================================================
CREATE
============================================================
*/

pdf_stream_t *
pdf_stream_create(void)
{
    pdf_stream_t *s =
        malloc(sizeof(pdf_stream_t));

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

void pdf_stream_destroy(pdf_stream_t *stream)
{
    if (!stream)
        return;

    cleaner_vector_destroy(stream->buffer);

    free(stream);
}

/*
============================================================
WRITE DATA
============================================================
*/

int pdf_stream_write(
    pdf_stream_t *stream,
    const void *data,
    size_t size)
{
    if (!stream || !data)
        return -1;

    const unsigned char *bytes =
        (const unsigned char *)data;

    for (size_t i = 0; i < size; i++)
    {
        cleaner_vector_push_back(
            stream->buffer,
            &bytes[i]);
    }

    return 0;
}

/*
============================================================
SIZE
============================================================
*/

size_t
pdf_stream_size(
    const pdf_stream_t *stream)
{
    if (!stream)
        return 0;

    return cleaner_vector_size(stream->buffer);
}

/*
============================================================
EMIT STREAM
============================================================
*/

int pdf_stream_emit(
    pdf_stream_t *stream,
    FILE *file)
{
    if (!stream || !file)
        return -1;

    size_t size =
        cleaner_vector_size(stream->buffer);

    fprintf(file, "<< /Length %zu >>\n", size);
    fprintf(file, "stream\n");

    for (size_t i = 0; i < size; i++)
    {
        unsigned char *b =
            cleaner_vector_at(stream->buffer, i);

        fwrite(b, 1, 1, file);
    }

    fprintf(file, "\nendstream\n");

    return 0;
}
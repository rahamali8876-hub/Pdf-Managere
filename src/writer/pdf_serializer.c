#include "writer/pdf_serializer.h"
#include "system/vector.h"

#include <stdlib.h>
#include <string.h>

struct pdf_serializer_s
{
    FILE *file;
};

/*
==========================================================
CREATE
==========================================================
*/

pdf_serializer_t *
pdf_serializer_create(FILE *file)
{
    pdf_serializer_t *ser =
        malloc(sizeof(pdf_serializer_t));

    if (!ser)
        return NULL;

    ser->file = file;

    return ser;
}

/*
==========================================================
DESTROY
==========================================================
*/

void pdf_serializer_destroy(
    pdf_serializer_t *ser)
{
    if (!ser)
        return;

    free(ser);
}

/*
==========================================================
NUMBER
==========================================================
*/

int pdf_write_number(
    pdf_serializer_t *ser,
    double value)
{
    return fprintf(
        ser->file,
        "%g",
        value);
}

/*
==========================================================
NAME
==========================================================
*/

int pdf_write_name(
    pdf_serializer_t *ser,
    const char *name)
{
    return fprintf(
        ser->file,
        "/%s",
        name);
}

/*
==========================================================
STRING
==========================================================
*/

int pdf_write_string(
    pdf_serializer_t *ser,
    const char *str)
{
    return fprintf(
        ser->file,
        "(%s)",
        str);
}

int pdf_write_array(
    pdf_serializer_t *ser,
    pdf_array_t *arr)
{
    fprintf(ser->file, "[");

    size_t count =
        pdf_array_size(arr);

    for (size_t i = 0; i < count; i++)
    {
        pdf_object_t *obj =
            pdf_array_get(arr, i);

        if (!obj)
            continue;

        pdf_write_number(
            ser,
            pdf_object_number(obj));

        if (i + 1 < count)
            fprintf(ser->file, " ");
    }

    fprintf(ser->file, "]");

    return 0;
}

int pdf_write_dictionary(
    pdf_serializer_t *ser,
    pdf_dictionary_t *dict)
{
    fprintf(ser->file, "<<\n");

    size_t count =
        pdf_dictionary_size(dict);

    for (size_t i = 0; i < count; i++)
    {
        /* dictionary iteration would be added here */
    }

    fprintf(ser->file, ">>\n");

    return 0;
}
#ifndef PDF_SERIALIZER_H
#define PDF_SERIALIZER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>

#include "core/pdf_object.h"
#include "core/pdf_array.h"
#include "core/pdf_dictionary.h"

    typedef struct pdf_serializer_s pdf_serializer_t;

    /*
    ==========================================================
    CREATE / DESTROY
    ==========================================================
    */

    pdf_serializer_t *
    pdf_serializer_create(FILE *file);

    void
    pdf_serializer_destroy(
        pdf_serializer_t *ser);

    /*
    ==========================================================
    WRITE OBJECT TYPES
    ==========================================================
    */

    int pdf_write_number(
        pdf_serializer_t *ser,
        double value);

    int pdf_write_name(
        pdf_serializer_t *ser,
        const char *name);

    int pdf_write_string(
        pdf_serializer_t *ser,
        const char *str);

    int pdf_write_array(
        pdf_serializer_t *ser,
        pdf_array_t *arr);

    int pdf_write_dictionary(
        pdf_serializer_t *ser,
        pdf_dictionary_t *dict);

#ifdef __cplusplus
}
#endif

#endif
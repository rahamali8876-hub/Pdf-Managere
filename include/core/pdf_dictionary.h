#ifndef PDF_DICTIONARY_H
#define PDF_DICTIONARY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include "core/pdf_object.h"

    typedef struct pdf_dictionary_s pdf_dictionary_t;

    /*
    ============================================================
    CREATE / DESTROY
    ============================================================
    */

    pdf_dictionary_t *
    pdf_dictionary_create(void);

    void
    pdf_dictionary_destroy(
        pdf_dictionary_t *dict);

    /*
    ============================================================
    INSERT
    ============================================================
    */

    int
    pdf_dictionary_set(
        pdf_dictionary_t *dict,
        const char *key,
        pdf_object_t *value);

    /*
    ============================================================
    LOOKUP
    ============================================================
    */

    pdf_object_t *
    pdf_dictionary_get(
        pdf_dictionary_t *dict,
        const char *key);

    size_t
    pdf_dictionary_size(
        const pdf_dictionary_t *dict);

#ifdef __cplusplus
}
#endif

#endif
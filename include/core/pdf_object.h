
#ifndef PDF_CORE_OBJECT_H
#define PDF_CORE_OBJECT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>

    /*
    ============================================================
    PDF OBJECT TYPES
    ============================================================
    */

    typedef enum
    {
        PDF_OBJECT_NULL = 0,
        PDF_OBJECT_INT,
        PDF_OBJECT_REAL,
        PDF_OBJECT_NAME,
        PDF_OBJECT_STRING,
        PDF_OBJECT_ARRAY,
        PDF_OBJECT_DICT

    } pdf_object_type_t;

    /*
    ============================================================
    PDF OBJECT
    ============================================================
    */

    typedef struct pdf_object_s pdf_object_t;

    /*
    ============================================================
    OBJECT CREATION
    ============================================================
    */

    pdf_object_t *
    pdf_object_create_int(int value);

    pdf_object_t *
    pdf_object_create_real(double value);

    pdf_object_t *
    pdf_object_create_name(const char *name);

    pdf_object_t *
    pdf_object_create_string(const char *value);

    /*
    ============================================================
    ARRAY OBJECT
    ============================================================
    */

    pdf_object_t *
    pdf_object_create_array(void);

    int
    pdf_object_array_add(
        pdf_object_t *array,
        pdf_object_t *value);

    /*
    ============================================================
    DICT OBJECT
    ============================================================
    */

    pdf_object_t *
    pdf_object_create_dict(void);

    int
    pdf_object_dict_set(
        pdf_object_t *dict,
        const char *key,
        pdf_object_t *value);

    /*
    ============================================================
    LIFETIME
    ============================================================
    */

    void
    pdf_object_destroy(pdf_object_t *obj);

    /* ADD PDF NUMBERS */

    int pdf_object_number(
        const pdf_object_t *obj);

    /*
    ============================================================
    SERIALIZATION
    ============================================================
    */

    int
    pdf_object_write(
        const pdf_object_t *obj,
        void *writer);

#ifdef __cplusplus
}
#endif

#endif
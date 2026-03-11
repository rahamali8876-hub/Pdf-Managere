// #ifndef PDF_CORE_OBJECT_H
// #define PDF_CORE_OBJECT_H

// #include <stddef.h>
// #include "system/vector.h"

// #ifdef __cplusplus
// extern "C"
// {
// #endif

//     typedef struct pdf_object_s pdf_object_t;
//     typedef struct pdf_dict_entry_s pdf_dict_entry_t;

//     /* ============================================================
//        OBJECT TYPE
//        ============================================================ */

//     typedef enum
//     {
//         PDF_OBJ_NULL = 0,
//         PDF_OBJ_BOOL,
//         PDF_OBJ_NUMBER,
//         PDF_OBJ_STRING,
//         PDF_OBJ_NAME,
//         PDF_OBJ_ARRAY,
//         PDF_OBJ_DICT,
//         PDF_OBJ_STREAM

//     } pdf_object_type_t;

//     /* ============================================================
//        DICTIONARY ENTRY
//        ============================================================ */

//     struct pdf_dict_entry_s
//     {
//         char *key;
//         pdf_object_t *value;
//     };

//     /* ============================================================
//        STREAM
//        ============================================================ */

//     typedef struct
//     {
//         unsigned char *data;
//         size_t length;

//     } pdf_stream_t;

//     /* ============================================================
//        OBJECT STRUCT
//        ============================================================ */

//     struct pdf_object_s
//     {
//         pdf_object_type_t type;

//         union
//         {
//             int boolean;
//             double number;

//             char *string;
//             char *name;

//             vector_t array;
//             vector_t dict;

//             pdf_stream_t stream;

//         } value;
//     };

//     /* ============================================================
//        API
//        ============================================================ */

//     pdf_object_t *pdf_object_new_null(void);
//     pdf_object_t *pdf_object_new_bool(int v);
//     pdf_object_t *pdf_object_new_number(double v);
//     pdf_object_t *pdf_object_new_string(const char *s);
//     pdf_object_t *pdf_object_new_name(const char *name);

//     pdf_object_t *pdf_object_new_array(void);
//     pdf_object_t *pdf_object_new_dict(void);

//     void pdf_object_destroy(pdf_object_t *obj);

// #ifdef __cplusplus
// }
// #endif

// #endif

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
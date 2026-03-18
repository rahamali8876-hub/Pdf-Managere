// // core/pdf_xref.h

// #ifndef PDF_CORE_XREF_H
// #define PDF_CORE_XREF_H

// #ifdef __cplusplus
// extern "C"
// {
// #endif

// #include <stdio.h>

// #include <stddef.h>

//     typedef struct pdf_xref_s pdf_xref_t;

//     /*
//     ============================================================
//     CREATE / DESTROY
//     ============================================================
//     */

//     pdf_xref_t *
//     pdf_xref_create(size_t initial_capacity);

//     void
//     pdf_xref_destroy(pdf_xref_t *xref);

//     /*
//     ============================================================
//     OBJECT REGISTRATION
//     ============================================================
//     */

//     int
//     pdf_xref_register(
//         pdf_xref_t *xref,
//         int object_number,
//         long offset);

//     /*
//     ============================================================
//     WRITE XREF TABLE
//     ============================================================
//     */

//     int
//     pdf_xref_write(
//         pdf_xref_t *xref,
//         FILE *file,
//         long xref_position);

// #ifdef __cplusplus
// }
// #endif

// #endif

#ifndef PDF_CORE_XREF_H
#define PDF_CORE_XREF_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stddef.h>

    /*
    ============================================================
    XREF ENTRY
    ============================================================
    */

    typedef struct
    {
        int object_number;
        long offset;

    } pdf_xref_entry_t;

    /*
    ============================================================
    XREF TABLE
    ============================================================
    */

    typedef struct pdf_xref_s pdf_xref_t;

    /*
    ============================================================
    CREATE / DESTROY
    ============================================================
    */

    pdf_xref_t *
    pdf_xref_create(size_t initial_capacity);

    void
    pdf_xref_destroy(pdf_xref_t *xref);

    /*
    ============================================================
    REGISTER OBJECT OFFSET
    ============================================================
    */

    int
    pdf_xref_register(
        pdf_xref_t *xref,
        int object_number,
        long offset);

    /*
    ============================================================
    WRITE XREF TABLE
    ============================================================
    */

    int
    pdf_xref_write(
        pdf_xref_t *xref,
        FILE *file,
        long xref_position);

#ifdef __cplusplus
}
#endif

#endif
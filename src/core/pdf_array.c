#include "core/pdf_array.h"
#include "system/vector.h"

#include <stdlib.h>

struct pdf_array_s
{
    cleaner_vector_t *elements;
};

/*
============================================================
CREATE
============================================================
*/

pdf_array_t *
pdf_array_create(void)
{
    pdf_array_t *arr =
        malloc(sizeof(pdf_array_t));

    if (!arr)
        return NULL;

    arr->elements =
        cleaner_vector_create(
            NULL,
            sizeof(pdf_object_t *),
            8);

    return arr;
}

/*
============================================================
DESTROY
============================================================
*/

void pdf_array_destroy(
    pdf_array_t *arr)
{
    if (!arr)
        return;

    cleaner_vector_destroy(arr->elements);

    free(arr);
}

/*
============================================================
PUSH
============================================================
*/

int pdf_array_push(
    pdf_array_t *arr,
    pdf_object_t *obj)
{
    if (!arr || !obj)
        return -1;

    return cleaner_vector_push_back(
        arr->elements,
        &obj);
}

/*
============================================================
GET
============================================================
*/

pdf_object_t *
pdf_array_get(
    pdf_array_t *arr,
    size_t index)
{
    if (!arr)
        return NULL;

    pdf_object_t **obj =
        cleaner_vector_at(
            arr->elements,
            index);

    if (!obj)
        return NULL;

    return *obj;
}

/*
============================================================
SIZE
============================================================
*/

size_t
pdf_array_size(
    const pdf_array_t *arr)
{
    if (!arr)
        return 0;

    return cleaner_vector_size(
        arr->elements);
}
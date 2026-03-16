// src/core/pdf_object_store.c

#include "core/pdf_object_store.h"
#include "system/vector.h"

#include <stdlib.h>

struct pdf_object_store_s
{
    cleaner_vector_t *objects;
};

/*
============================================================
CREATE
============================================================
*/

pdf_object_store_t *
pdf_object_store_create(void)
{
    pdf_object_store_t *store =
        malloc(sizeof(pdf_object_store_t));

    if (!store)
        return NULL;

    store->objects =
        cleaner_vector_create(
            NULL,
            sizeof(pdf_object_t *),
            64);

    return store;
}

/*
============================================================
DESTROY
============================================================
*/

void pdf_object_store_destroy(
    pdf_object_store_t *store)
{
    if (!store)
        return;

    size_t count =
        cleaner_vector_size(store->objects);

    for (size_t i = 0; i < count; i++)
    {
        pdf_object_t **obj =
            cleaner_vector_at(store->objects, i);

        if (obj && *obj)
            pdf_object_destroy(*obj);
    }

    cleaner_vector_destroy(store->objects);

    free(store);
}

/*
============================================================
ADD OBJECT
============================================================
*/

int pdf_object_store_add(
    pdf_object_store_t *store,
    pdf_object_t *obj)
{
    if (!store || !obj)
        return -1;

    return cleaner_vector_push_back(
        store->objects,
        &obj);
}

/*
============================================================
GET OBJECT BY NUMBER
============================================================
*/

pdf_object_t *
pdf_object_store_get(
    pdf_object_store_t *store,
    int object_number)
{
    if (!store)
        return NULL;

    size_t count =
        cleaner_vector_size(store->objects);

    for (size_t i = 0; i < count; i++)
    {
        pdf_object_t **obj =
            cleaner_vector_at(store->objects, i);

        if (*obj &&
            pdf_object_number(*obj) == object_number)
        {
            return *obj;
        }
    }

    return NULL;
}

/*
============================================================
COUNT
============================================================
*/

size_t
pdf_object_store_count(
    const pdf_object_store_t *store)
{
    if (!store)
        return 0;

    return cleaner_vector_size(store->objects);
}

/*
============================================================
INDEX ACCESS
============================================================
*/

pdf_object_t *
pdf_object_store_at(
    pdf_object_store_t *store,
    size_t index)
{
    if (!store)
        return NULL;

    pdf_object_t **obj =
        cleaner_vector_at(
            store->objects,
            index);

    if (!obj)
        return NULL;

    return *obj;
}
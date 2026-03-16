#include "core/pdf_object.h"
#include "system/vector.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
    char *key;
    pdf_object_t *value;

} pdf_dict_entry_t;

struct pdf_object_s
{
    pdf_object_type_t type;

    union
    {
        int int_value;
        double real_value;

        char *string_value;

        cleaner_vector_t *array;

        cleaner_vector_t *dict;

    } data;
};

/*
============================================================
UTILITY
============================================================
*/

static char *
dup_string(const char *s)
{
    if (!s)
        return NULL;

    size_t len = strlen(s);

    char *copy = malloc(len + 1);

    memcpy(copy, s, len + 1);

    return copy;
}

/*
============================================================
CREATE BASIC OBJECTS
============================================================
*/

pdf_object_t *
pdf_object_create_int(int value)
{
    pdf_object_t *obj = malloc(sizeof(pdf_object_t));

    obj->type = PDF_OBJECT_INT;
    obj->data.int_value = value;

    return obj;
}

pdf_object_t *
pdf_object_create_real(double value)
{
    pdf_object_t *obj = malloc(sizeof(pdf_object_t));

    obj->type = PDF_OBJECT_REAL;
    obj->data.real_value = value;

    return obj;
}

pdf_object_t *
pdf_object_create_name(const char *name)
{
    pdf_object_t *obj = malloc(sizeof(pdf_object_t));

    obj->type = PDF_OBJECT_NAME;
    obj->data.string_value = dup_string(name);

    return obj;
}

pdf_object_t *
pdf_object_create_string(const char *value)
{
    pdf_object_t *obj = malloc(sizeof(pdf_object_t));

    obj->type = PDF_OBJECT_STRING;
    obj->data.string_value = dup_string(value);

    return obj;
}

/*
============================================================
ARRAY
============================================================
*/

pdf_object_t *
pdf_object_create_array(void)
{
    pdf_object_t *obj = malloc(sizeof(pdf_object_t));

    obj->type = PDF_OBJECT_ARRAY;

    obj->data.array =
        cleaner_vector_create(NULL, sizeof(pdf_object_t *), 8);

    return obj;
}

int pdf_object_array_add(
    pdf_object_t *array,
    pdf_object_t *value)
{
    if (!array || array->type != PDF_OBJECT_ARRAY)
        return -1;

    return cleaner_vector_push_back(
        array->data.array,
        &value);
}

/*
============================================================
DICT
============================================================
*/

pdf_object_t *
pdf_object_create_dict(void)
{
    pdf_object_t *obj = malloc(sizeof(pdf_object_t));

    obj->type = PDF_OBJECT_DICT;

    obj->data.dict =
        cleaner_vector_create(NULL,
                              sizeof(pdf_dict_entry_t),
                              8);

    return obj;
}

int pdf_object_dict_set(
    pdf_object_t *dict,
    const char *key,
    pdf_object_t *value)
{
    if (!dict || dict->type != PDF_OBJECT_DICT)
        return -1;

    pdf_dict_entry_t entry;

    entry.key = dup_string(key);
    entry.value = value;

    return cleaner_vector_push_back(
        dict->data.dict,
        &entry);
}

/*
============================================================
DESTROY
============================================================
*/

void pdf_object_destroy(pdf_object_t *obj)
{
    if (!obj)
        return;

    switch (obj->type)
    {
    case PDF_OBJECT_STRING:
    case PDF_OBJECT_NAME:
        free(obj->data.string_value);
        break;

    case PDF_OBJECT_ARRAY:
    {
        size_t count =
            cleaner_vector_size(obj->data.array);

        for (size_t i = 0; i < count; i++)
        {
            pdf_object_t **v =
                cleaner_vector_at(obj->data.array, i);

            if (v)
                pdf_object_destroy(*v);
        }

        cleaner_vector_destroy(obj->data.array);
        break;
    }

    case PDF_OBJECT_DICT:
    {
        size_t count =
            cleaner_vector_size(obj->data.dict);

        for (size_t i = 0; i < count; i++)
        {
            pdf_dict_entry_t *e =
                cleaner_vector_at(obj->data.dict, i);

            free(e->key);
            pdf_object_destroy(e->value);
        }

        cleaner_vector_destroy(obj->data.dict);
        break;
    }

    default:
        break;
    }

    free(obj);
}

int pdf_object_number(
    const pdf_object_t *obj)
{
    if (!obj)
        return -1;

    if (obj->type == PDF_OBJECT_INT)
        return obj->data.int_value;
    else if (obj->type == PDF_OBJECT_REAL)
        return (int)obj->data.real_value;
    else
        return -1;
}
#include "core/pdf_dictionary.h"
#include "system/vector.h"

#include <stdlib.h>
#include <string.h>

typedef struct
{
    char *key;
    pdf_object_t *value;

} pdf_dict_entry_t;

struct pdf_dictionary_s
{
    cleaner_vector_t *entries;
};

/*
============================================================
CREATE
============================================================
*/

pdf_dictionary_t *
pdf_dictionary_create(void)
{
    pdf_dictionary_t *dict =
        malloc(sizeof(pdf_dictionary_t));

    if (!dict)
        return NULL;

    dict->entries =
        cleaner_vector_create(
            NULL,
            sizeof(pdf_dict_entry_t),
            8);

    return dict;
}

/*
============================================================
DESTROY
============================================================
*/

void pdf_dictionary_destroy(
    pdf_dictionary_t *dict)
{
    if (!dict)
        return;

    size_t count =
        cleaner_vector_size(dict->entries);

    for (size_t i = 0; i < count; i++)
    {
        pdf_dict_entry_t *e =
            cleaner_vector_at(
                dict->entries,
                i);

        free(e->key);
    }

    cleaner_vector_destroy(dict->entries);

    free(dict);
}

/*
============================================================
SET
============================================================
*/

int pdf_dictionary_set(
    pdf_dictionary_t *dict,
    const char *key,
    pdf_object_t *value)
{
    if (!dict || !key)
        return -1;

    pdf_dict_entry_t entry;

    entry.key = strdup(key);
    entry.value = value;

    return cleaner_vector_push_back(
        dict->entries,
        &entry);
}

/*
============================================================
GET
============================================================
*/

pdf_object_t *
pdf_dictionary_get(
    pdf_dictionary_t *dict,
    const char *key)
{
    if (!dict)
        return NULL;

    size_t count =
        cleaner_vector_size(dict->entries);

    for (size_t i = 0; i < count; i++)
    {
        pdf_dict_entry_t *e =
            cleaner_vector_at(
                dict->entries,
                i);

        if (strcmp(e->key, key) == 0)
            return e->value;
    }

    return NULL;
}

/*
============================================================
SIZE
============================================================
*/

size_t
pdf_dictionary_size(
    const pdf_dictionary_t *dict)
{
    if (!dict)
        return 0;

    return cleaner_vector_size(
        dict->entries);
}
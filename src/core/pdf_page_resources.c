#include "core/pdf_page_resources.h"
#include "system/vector.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
============================================================
RESOURCE ENTRY
============================================================
*/

typedef struct
{
    int object_number;
    char name[16]; /* Im0, Im1 */

} pdf_resource_entry_t;

/*
============================================================
STRUCTURE
============================================================
*/

struct pdf_page_resources_s
{
    cleaner_vector_t *images;
    int counter;
};

/*
============================================================
CREATE
============================================================
*/

pdf_page_resources_t *
pdf_page_resources_create(void)
{
    pdf_page_resources_t *res =
        (pdf_page_resources_t *)malloc(
            sizeof(pdf_page_resources_t));

    if (!res)
        return NULL;

    res->images =
        cleaner_vector_create(
            NULL,
            sizeof(pdf_resource_entry_t),
            8);

    if (!res->images)
    {
        free(res);
        return NULL;
    }

    res->counter = 0;

    return res;
}

/*
============================================================
DESTROY
============================================================
*/

void pdf_page_resources_destroy(
    pdf_page_resources_t *res)
{
    if (!res)
        return;

    cleaner_vector_destroy(res->images);

    free(res);
}

/*
============================================================
ADD IMAGE
============================================================
*/

const char *
pdf_page_resources_add_image(
    pdf_page_resources_t *res,
    int object_number)
{
    if (!res)
        return NULL;

    pdf_resource_entry_t entry;

    entry.object_number = object_number;

    snprintf(
        entry.name,
        sizeof(entry.name),
        "Im%d",
        res->counter++);

    if (cleaner_vector_push_back(
            res->images,
            &entry) != 0)
    {
        return NULL;
    }

    pdf_resource_entry_t *stored =
        cleaner_vector_at(
            res->images,
            cleaner_vector_size(res->images) - 1);

    return stored->name;
}

/*
============================================================
WRITE XOBJECT DICTIONARY
============================================================
*/

static void
pdf_write_xobject_dict(
    pdf_page_resources_t *res,
    FILE *file)
{
    size_t count =
        cleaner_vector_size(res->images);

    fprintf(file, "/XObject <<\n");

    for (size_t i = 0; i < count; i++)
    {
        pdf_resource_entry_t *e =
            cleaner_vector_at(res->images, i);

        fprintf(file,
                "/%s %d 0 R\n",
                e->name,
                e->object_number);
    }

    fprintf(file, ">>\n");
}

/*
============================================================
WRITE RESOURCES
============================================================
*/

int pdf_page_resources_write(
    pdf_page_resources_t *res,
    FILE *file)
{
    if (!res || !file)
        return -1;

    fprintf(file, "<<\n");

    if (cleaner_vector_size(res->images) > 0)
    {
        pdf_write_xobject_dict(res, file);
    }

    fprintf(file, ">>\n");

    return 0;
}

/*
============================================================
COUNT
============================================================
*/

int pdf_page_resources_count(
    const pdf_page_resources_t *res)
{
    if (!res)
        return 0;

    return (int)cleaner_vector_size(res->images);
}
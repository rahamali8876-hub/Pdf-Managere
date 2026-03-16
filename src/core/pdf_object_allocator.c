#include "core/pdf_object_allocator.h"

void pdf_object_allocator_init(
    pdf_object_allocator_t *alloc)
{
    alloc->next_id = 1;
}

int pdf_object_alloc(
    pdf_object_allocator_t *alloc)
{
    return alloc->next_id++;
}
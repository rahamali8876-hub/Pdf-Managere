#ifndef PDF_OBJECT_ALLOCATOR_H
#define PDF_OBJECT_ALLOCATOR_H

typedef struct
{
    int next_id;

} pdf_object_allocator_t;

void pdf_object_allocator_init(
    pdf_object_allocator_t *alloc);

int pdf_object_alloc(
    pdf_object_allocator_t *alloc);

#endif
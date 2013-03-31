#ifndef _DS_ALLOC_H
#define _DS_ALLOC_H


void *ds_alloc(size_t size);
void *ds_calloc(size_t size);
void *ds_memalign(size_t alignment, size_t size);

#define ds_free free

#endif

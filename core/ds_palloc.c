#include "ds.h"
#include "ds_log.h"
#include "ds_palloc.h"
#include "ds_alloc.h"


ds_pool_t* ds_create_pool(size_t size)
{
	ds_pool_t *p;

	p = ds_memalign(DS_POOL_ALIGNMENT, size);

	if (p == NULL)
	{
		return NULL;
	}

	p->d.last = (char *)p + sizeof(ds_pool_t);
	p->d.end = (char *)p + size;
	p->d.next = NULL;
	p->d.failed = 0;

	size = size - sizeof(ds_pool_t);
	p->max = (size < DS_MAX_ALLOC_FROM_POOL) ? size : DS_MAX_ALLOC_FROM_POOL;

	p->current = p;
	p->cleanup = NULL;
	p->large = NULL;
	
	return p;
}
#include "ds.h"
#include "ds_log.h"
#include "ds_alloc.h"

void *ds_alloc(size_t size)
{
	void *p = NULL;

	p = malloc(size);

	if (NULL == p)
	{
		ds_log_error("malloc size[%lu]failed!", size);
	}
	
	return p;
}

void *ds_calloc(size_t size)
{
	void *p = NULL;

	p = ds_alloc(size);

	if (p) 
	{
		memset(p, 0, size);
	}

	return p;
}

void *ds_memalign(size_t alignment,size_t size)
{
	void *p;
	int err;

	err = posix_memalign(&p, alignment, size);

	if (err)
	{
		ds_log_error("posix memalign failed!");
		p = NULL;
	}

	return p;
}
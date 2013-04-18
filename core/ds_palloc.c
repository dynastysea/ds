#include "../common/ds.h"
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




void ds_destroy_pool(ds_pool_t *pool)
{
	ds_pool_t *p, *n;
	ds_pool_cleanup_t *c;
	ds_pool_large_t *l;

	for (c = pool->cleanup; c; c = c->next)
	{
		if (c->handler)
		{
			c->handler(c->data);
		}
	}

	for (l = pool->large; l; l = l->next)
	{
		if (l->alloc)
		{
			ds_free(l->alloc);
		}
	}

	for (p = pool, n = p->d.next; ; p = n, n = n->d.next)
	{
		ds_free(p);
		
		if (n == NULL)
		{
			break;
		}
	}
	
}

void ds_reset_pool(ds_pool_t *pool)
{
	ds_pool_t *p;
	ds_pool_large_t *l;

	for (l = pool->large; l; l = l->next)
	{
		if (l->alloc)
		{
			ds_free(l->alloc);
		}
	}

	pool->large = NULL;
	
	for (p = pool; p; p = pool->d.next)
	{
		p->d.last = (char *)p + sizeof(ds_pool_t);
	}

}

static void *ds_palloc_block(ds_pool_t *pool, size_t size)
{
	char *m;
	size_t p_size;
	ds_pool_t *p, *new, *current;

	p_size = (size_t)(pool->d.end - (char *)pool);

	m = ds_memalign(DS_POOL_ALIGNMENT, p_size);
	if (m == NULL)
	{
		return NULL;
	}

	new = (ds_pool_t *)m;

	new->d.next = NULL;
	new->d.end = m + p_size;
	new->d.failed = 0;

	m += sizeof(ds_pool_data_t);
	m = ds_align_ptr(m, DS_ALIGNMENT);
	new->d.last = m + size;
	
	current = pool->current;

	for (p = current; p->d.next; p = p->d.next)
	{
		if (p->d.failed++ > 4)
		{
			current = p->d.next;
		}
	}

	p->d.next = new;

	pool->current = current ? current : new;
	
	return m;
}

static void *ds_palloc_large(ds_pool_t *pool, size_t size)
{
	void *p;
	int n;
	ds_pool_large_t *large;

	p = ds_alloc(size);
	if (NULL == p)
	{
		return NULL;
	}

	n = 0;

	for (large = pool->large; large; large = large->next)
	{
		if (large->alloc == NULL)
		{
			large->alloc = p;
			return p;
		}

		if (n++ > 3)
		{
			break;
		}

	}

	large = ds_alloc(sizeof(ds_pool_large_t));

	if (NULL == large)
	{
		ds_free(p);
		return NULL;
	}

	large->alloc = p;
	large->next = pool->large;

	pool->large = large;

	return p;
}

void *ds_palloc(ds_pool_t *pool, size_t size)
{
	ds_pool_t *p;
	char *m;

	if (size <= pool->max)
	{
		p = pool->current;

		do 
		{
			m = ds_align_ptr(p->d.last, DS_ALIGNMENT);

			if ((size_t)(p->d.end - m) >= size)
			{
				p->d.last = m + size;
				return m;
			}

			p = p->d.next;
		}while(p);		

		return ds_palloc_block(pool, size);
	}

	return ds_palloc_large(pool, size);
}

void *ds_pcalloc(ds_pool_t *pool, size_t size)
{
	void *p;

	p = ds_palloc(pool, size);
	if (p)
	{
		memset(p, 0, size);
	}

	return p;
}


int ds_pfree(ds_pool_t *pool, void *p)
{
	ds_pool_large_t *large;

	for (large = pool->large; large; large = large->next)
	{
		if (p == large->alloc)
		{
			ds_free(large->alloc);
			large->alloc = NULL;

			return DS_OK;
		}

	}

	return DS_DECLINED;
}

void *ds_pmemalign(ds_pool_t *pool, size_t size, size_t alignment)
{
    void              *p;
    ds_pool_large_t  *large;

    p = ds_memalign(alignment, size);
    if (p == NULL) {
        return NULL;
    }

    large = ds_palloc(pool, sizeof(ds_pool_large_t));
    if (large == NULL) {
        ds_free(p);
        return NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}



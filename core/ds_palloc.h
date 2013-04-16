#ifndef _DS_PALLOC_H
#define _DS_PALLOC_H

#define DS_POOL_ALIGNMENT 16
#define DS_MAX_ALLOC_FROM_POOL (getpagesize() - 1)

typedef struct ds_pool ds_pool_t;
typedef struct ds_pool_large ds_pool_large_t;

typedef void (*ds_pool_cleanup_pt)(void *data);

struct ds_pool_large
{
	ds_pool_large_t	*next;
	void 			*alloc;
};

typedef struct ds_pool_cleanup
{
	ds_pool_cleanup_pt 		handler;
	void 					*data;
	struct ds_pool_cleanup  *next;
}ds_pool_cleanup_t;

typedef struct ds_pool_data
{
	char 		*last;
	char 		*end;
	ds_pool_t 	*next;
	int failed;
}ds_pool_data_t;


struct ds_pool
{
	ds_pool_data_t 		d;
	size_t 				max;
	ds_pool_t 			*current;
	ds_pool_large_t 	*large;
	ds_pool_cleanup_t 	*cleanup;	
};

ds_pool_t *ds_create_pool(size_t size);
void ds_destroy_pool(ds_pool_t *pool);
void ds_reset_pool(ds_pool_t *pool);


void *ds_palloc(ds_pool_t *pool, size_t size);
void *ds_pcalloc(ds_pool_t *pool, size_t size);
int ds_pfree(ds_pool_t *pool, void *p);
void *ds_pmemalign(ds_pool_t *pool, size_t size, size_t alignment);


#endif


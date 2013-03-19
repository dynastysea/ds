#ifndef _LIST_H
#define _LIST_H

/*
 * Simple doubly linked list implementation.
 * reference the linux list 
 */


/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({			\
		const typeof(((type *)0)->member) * __mptr = (ptr); \
		(type *)((char *)__mptr - offsetof(type, member)); })

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
		container_of(ptr, type, member)


typedef struct list_head
{
	struct list_head *next;
	struct list_head *prev;
}list_head_t;

static inline void init_list_head(list_head_t *list)
{
	list->next = list;
	list->prev = list;
}

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */

static inline void __list_add(list_head_t *new, list_head_t *prev, list_head_t *next)
{	
	next->prev = new;
	new->prev = prev;
	new->next = next;
	prev->next = new;
}
/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */

static inline void list_add(list_head_t *new, list_head_t *head)
{
	__list_add(new, head,head->next);
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(list_head_t *new, list_head_t *head)
{
	__list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */

static inline void __list_del(list_head_t *prev, list_head_t *next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void __list_del_entry(list_head_t *entry)
{
	__list_del(entry->prev, entry->next);
}

static inline void list_del(list_head_t *entry)
{
	__list_del(entry->prev, entry->next);
	entry->prev = NULL;
	entry->next = NULL;
}

/**
 * list_replace - replace old entry by new one
 * @old : the element to be replaced
 * @new : the new element to insert
 *
 * If @old was empty, it will be overwritten.
 */

static inline void list_replace(list_head_t *old, list_head_t *new)
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}

static inline void list_replace_init(list_head_t *old,
					list_head_t *new)
{
	list_replace(old, new);
	init_list_head(old);
}

/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void list_del_init(list_head_t *entry)
{
	__list_del_entry(entry);
	init_list_head(entry);
}

/**
 * list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void list_move(list_head_t *list, list_head_t *head)
{
	__list_del_entry(list);
	list_add(list, head);
}

/**
 * list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void list_move_tail(list_head_t *list,
				  list_head_t *head)
{
	__list_del_entry(list);
	list_add_tail(list, head);
}

/**
 * list_is_last - tests whether @list is the last entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
static inline int list_is_last(const list_head_t *list,
				const list_head_t *head)
{
	return list->next == head;
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int list_empty(const list_head_t *head)
{
	return head->next == head;
}



#endif

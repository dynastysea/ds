#ifndef _DS_STRING_H
#define _DS_STRING_H

#include "ds.h"

typedef struct 
{
	size_t len;
	u_char *data;
}ds_string_t;


#define ds_string(str) {sizeof(str) - 1, (u_char *)str}
#define ds_null_string {0, NULL}


/*text must be constant string*/
#define ds_str_set(str, text) 			\
	(str)->len = sizeof(text) - 1; (str)->data = (u_char *)text
#define ds_str_null(str) (str)->len = 0; (str)->data = NULL

#define ds_tolower(c)  (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define ds_toupper(c)  (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

#define ds_strncmp(s1, s2, n) 	strncmp((const char *)s1, (const char *)s2, n)
#define ds_strcmp(s1, s2) 	strcmp((const char *)s1, (const char *)s2)

#define ds_strstr(s1, s2) strstr((const char *)s1, (const char *)s2)
#define ds_strchr(s1, c) strchr((const char *) s1, (int) c)

#define ds_strlen(s) strlen((const char *)s)


void ds_strlow(u_char *dst, u_char *src, size_t n);

static inline u_char * ds_strlchr(u_char *p, u_char *last, u_char c)
{
	while (p < last)
	{
		if (*p == c)
		{
			return p;
		}
		p++;
	}

	return NULL;
}



#endif


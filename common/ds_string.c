#include "ds_string.h"

void ds_strlow(u_char * dst,u_char * src,size_t n)
{
	while(n)
	{
		*dst = ds_tolower(*src);

		dst++;
		src++;
		n--;
	}
}

int ds_strcasecmp(u_char * s1, u_char * s2)
{
	intptr_t c1, c2;

	for( ;; )
	{
		c1 = (intptr_t)*s1++;
		c2 = (intptr_t)*s2++;
		
		c1 = ((c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1);
		c2 = ((c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2);

		if (c1 == c2)
		{
			if (c1)
			{
				continue;
			}

			return 0;
		}

		return c1 - c2;
	}

}

int ds_strncasecmp(u_char *s1, u_char *s2, size_t n)
{
	intptr_t c1, c2;

	while(n)
	{
		c1 = (intptr_t)*s1++;
		c2 = (intptr_t)*s2++;

		c1 = ((c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1);
		c2 = ((c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2);

		if (c1 == c2)
		{
			if (c1)
			{
				n--;
				continue;
			}

			return 0;
		}

		return c1 - c2;


	}

	return 0;
}


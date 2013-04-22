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



#include <gkstring.h>

#include "errormess.proto.h"

void ErrorMess(char *s)
{
	fprintf(stderr,"%s\n", s );
}

/*
int sprintf(dest, fmt)
char    *dest;		// pointer to buffer space
char    *fmt;		// pointer to format string
{
	return xprintf(dest, &fmt,false);
}
*/

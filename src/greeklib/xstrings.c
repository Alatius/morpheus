#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xstrings.proto.h"
#include "Fclose.proto.h"

int Xstrncpy(char *s1, const char *s2, size_t len)
{
	if (len == 0) return 1;
	size_t slen = strlen(s2);
	if (slen >= len) slen = len - 1;
	memmove(s1, s2, slen);
	s1[slen] = '\0';
	return 1;
}


void Xstrncat(char *s1, const char *s2, size_t len)
{
	size_t nlen;
	
	if( len  < 5 || len > BUFSIZ*4 ) {
		fprintf(stderr,"Xstrncat: hey! len %zu for [%s] \n", len, s2 );
	}

	if( (size_t)(Xstrlen(s1) + Xstrlen(s2)) > len - 1 ) {
		fprintf(stderr,"limit: %zu; tacking [%s] + [%s] is too big!\n", len , s1 , s2 );
		nlen = len - Xstrlen(s1) - 1;
		fprintf(stderr,"nlen %zu\n", nlen );
		strncat(s1,s2,(size_t)nlen);
		*(s1+len-1) = 0;
	} else
		strcat(s1,s2);
}

int Xstrncmp(const char *s1, const char *s2, size_t len)
{
	return(strncmp(s1,s2,(size_t)len));
}

int Xstrlen(const char *s)
{
	return((int)strlen(s));
}

/*
strcpy(char *s1, const char *s2)
{
	while(*s2) {
		*s1++ = *s2++;
	}
	*s1 = 0;
}
*/

#include <greek.h>

#include "beta_tolower.proto.h"
/*
 * convert from beta code capital letters
 *
 * greg crane
 * may 1988
 */

/*
 * start with something like "*)/andra"
 * and end with "a)/ndra"
 */
void beta_tolower(char *word)
{
	register char * s;
	register char * t;

	if( *word != BETA_UCASE_MARKER ) return;

	s = word;
	while(!isalpha(*s)&&*s) s++;
	*word = *s;
	memmove(s,s+1,strlen(s+1)+1);

}

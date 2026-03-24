#include <greek.h>

#include "normucase.proto.h"
/*
 * convert from beta code capital letters
 *
 * greg crane
 * february 1987
 */

/*
 * start with something like "*(/ellhn"
 * and end with "E(/llhn"
 */
void normucase(char *word)
{
	register char * s;

	if( *word != BETA_UCASE_MARKER ) return;

	s = word;
	while(!isalpha(*s)&&*s) s++;

	/*
	 * in case of "*(/ellhn", s points now to "ellhn"
 	 */
	if( ! islower(*s) )
		return;
	*word = toupper(*s);
	/*
 	 * word now "E(/ellhn"
	 */
	memmove(s, s+1, strlen(s+1)+1);
	/*
 	 * word now "E(/llhn"
	 */
}

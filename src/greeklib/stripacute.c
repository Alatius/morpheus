#include <greek.h>

#include "stripacute.proto.h"
#include "strsqz.proto.h"

/*
 * gregory crane
 *
 * harvard university
 */
 
void stripacute(char *s)
{
	while(*s) {
		if( *s == ACUTE ) 
				strsqz(s,1);
		s++;
	}
}
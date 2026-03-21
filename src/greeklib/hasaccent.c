/*
 * grc 9/13/88
 *
 */

#include <greek.h>

#include "hasaccent.proto.h"

int hasaccent(char *s)
{
	while(*s) {
		if( Is_accent(*s) ) return(1);
		s++;
	}
	return(0);
}
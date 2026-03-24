/*
 *	greg crane
 *	
 *	february 1987
 */

#include <greek.h>

#include "stripdiaer.proto.h"
#include "strsqz.proto.h"
#include "xstrings.proto.h"

void stripdiaer(char *word)
{
	register char *p;

	for (p = lastn(word,1);p >= word;p--)
		if (*p == DIAERESIS )
			strsqz(p,1);
}

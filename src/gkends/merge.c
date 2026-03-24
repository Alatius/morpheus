#include <gkstring.h>

#include "merge.proto.h"
#include "retrends.proto.h"

/*
 *
 */
int merge_keys(gk_string *Have, gk_string *Avoid, char *stem, char *endstr, char *newkeys)
{
	int rval = 0;

	if( EndingOk(newkeys,Have,Avoid,0) ) {
		rval = 1;
	}

	return(rval);
}

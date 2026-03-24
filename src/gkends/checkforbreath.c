#include <gkstring.h>

#include "checkforbreath.proto.h"
#include "../morphlib/morphflags.proto.h"
#include "../morphlib/setlang.proto.h"
#include "../greeklib/addbreath.proto.h"
#include "../greeklib/getbreath.proto.h"

void CheckForBreathing(gk_string *gstr)
{
	char * s = gkstring_of(gstr);
	int rbreath = 0;

	if( cur_lang() == LATIN || cur_lang() == ITALIAN) return;
	if( (rbreath=has_morphflag(morphflags_of(gstr),NEEDS_RBREATH)) )
		zap_morphflag(morphflags_of(gstr),NEEDS_RBREATH);
	if( ! Is_vowel(*s)  || getbreath(s) != NOBREATH ) {
		return;
	}
	addbreath(s,rbreath ? ROUGHBR : SMOOTHBR);

}

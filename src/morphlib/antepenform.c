#include <gkstring.h>

#include "antepenform.proto.h"
#include "morphflags.proto.h"

int antepen_form(gk_string *gstring, word_form form_info)
{
	(void)form_info;

	if( Is_antepen_accent(morphflags_of(gstring)) ) {
		return(1);
	}
	return(0);
}

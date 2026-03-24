#include <contract.h>
#include "../morphlib/morphkeys.proto.h"
#include "../greeklib/endsinstr.proto.h"
#include "../greeklib/xstrings.proto.h"
#include "ppasstype.proto.h"

gk_string * ppass_table = NULL;
static int nppass = 0;

void makeppass(char *origstem, gk_string *gstr)
{
	char newstem[MAXWORDSIZE];
	char stemname[MAXWORDSIZE];


	if( ! get_ppasstype(origstem,newstem,stemname) ) return;
	Xstrncpy(origstem,newstem,MAXWORDSIZE);
	set_stemtype(gstr,GetStemNum(stemname));
}

int get_ppasstype(char *stem, char *newstem, char *stemname)
{
	int  i;
	char * p;
	
	if( Is_vowel(*(lastn(stem,1)))  )
		return(0);
	Xstrncpy(newstem,stem,MAXWORDSIZE);
	Xstrncpy(stemname,"perfp_vow",MAXWORDSIZE);
	if( ! ppass_table ) {
		ppass_table = load_euph_tab(PPASSLIST,&nppass,NO);

		if( ! ppass_table)  return(0);
	}
	for(i=0;i<nppass;i++) {
		p = gkstring_of(ppass_table+i);
		if( ends_in(stem,p) ) {
			*(lastn(newstem,strlen(p))) = 0;
			Xstrncpy(stemname,p+MAXSUBSTRING,MAXWORDSIZE);

			return(1);
		}

	}
	return(0);
}



#include <gkstring.h>

#include "checkstem.proto.h"

int checkcmpstem(char *s, char *t, FILE *f)
{
	(void)t;
	char * p;
	char tmp[2056];
	char endkeys[LONGSTRING];
	char stemkeys[LONGSTRING];
	int rval = 0;
	
	strcpy(tmp,s);
	p = tmp;
	while(*p) p++;
	p--;
	
	if( isdigit(*s) ) return(0);
	strcpy(endkeys,"os_ou os_h_on os_on h_hs a_hs");
	while(p>tmp) {
		if( *p == 'o' || *p == 'h' ) {
			*p = 0;
			rval = stemexists(tmp, endkeys, stemkeys, 1);
			if( rval && strlen(s+strlen(tmp)+1) > 2) {
				fprintf(f,"%s\t%s\n" , s+strlen(tmp)+1, tmp);
				fprintf(stdout,"%s\t%s\n" , s+strlen(tmp)+1, tmp);
			}
		}
		p--;
	}
}

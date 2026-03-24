#include <greek.h>

#include "zap2ndbreath.proto.h"
#include "xstrings.proto.h"

/*
 * "e)nh(/bwsa" --> "e)nh/bwsa"
 * "parh(/bwsa" --> "parh/bwsa"
 */

void zap_extra_breath(char *s)
{
	int breath_is_extra = 0;
	
	if( *s == 'r' && *(s+1) == ROUGHBR ) s += 2;
	
	while(*s) {
	
		if( Is_cons(*s) ) {
			breath_is_extra++;
			s++;
			continue;
		}
		
		if( *s == ROUGHBR || *s == SMOOTHBR  ) {
			if( ! breath_is_extra ) {
				breath_is_extra++;
				s++;
				continue;
			}
			memmove(s,s+1,strlen(s+1)+1);
		}
		s++;
	}
}

int has_extra_breath(char *s)
{	
	int breath_is_extra = 0;
	while(*s) {
		if( Is_cons(*s) ) {
			breath_is_extra++;
			s++;
			continue;
		}
	
		if( *s == ROUGHBR || *s == SMOOTHBR) {
			if( ! breath_is_extra ) {
				breath_is_extra++;
				s++;
				continue;
			}
			return(1);
		}
		s++;
	}
	return(0);
}

/*
 * grc 11/17/94
 *
 * e.g., go from a)nanti/r)r(hton to a)nanti/rrhton
 */
void zap_rr_breath(char *s)
{
	s++; /* careful about r(i/ptw etc. */
	while(*s) {
		if( *s=='r' && *(s+1) == ')' 
		&&  *(s+2)=='r' && *(s+3) == '(' ) {
			Xstrncpy(s,"rr",MAXWORDSIZE);
			memmove(s+2,s+4,strlen(s+4)+1);
		}
		s++;
	}
}

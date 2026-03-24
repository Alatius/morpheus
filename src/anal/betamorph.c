#include <gkstring.h>

#include "betamorph.proto.h"
#include "prntanal.proto.h"
#include "checkstring.proto.h"

int quickflag;

_main(void)
{
	char line[BUFSIZ];
	PrntFlags prntflags  = KEEP_BETA;
	char * s;
	int nverbs = 0;
	int rval;
	
	while(fgets(line, sizeof line, stdin)) {
		line[strcspn(line, "\n")] = '\0';
		if(  strncmp(line,":le:",4)) {
			continue;
		}
		s = line+4;
		rval = checkstring(s,prntflags);
		printf("%s\t%d\n", s , rval );
		if( ++nverbs > 10 ) break;
		
	}
}
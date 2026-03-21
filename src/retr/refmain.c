/*
 * Copyright Gregory Crane
 * April 1987
 */

#include "srchstate.h"
#include "../auto/devices.h"

srch_state * srch;
char line[BUFSIZ];

main(argc,argv)
int argc;
char * argv[];
{
	while(fgets(line, sizeof line, stdin)) {
		line[strcspn(line, "\n")] = '\0';
		srch = (srch_state *) calloc(1, sizeof * srch );
		parseref(srch,line);
		gk_startup(BETAFORMAT);
		OpTxtFile(srch->sname);
		SaveFromTo(srch,srch->pstart,srch->pend,stdout);
		ClTxtFile(srch->sname);
		printf("\n\n");
		
		free(srch);
	}
}

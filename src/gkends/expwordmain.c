#include <gkstring.h>
char fname[80];
#include "compostypes.h"

#include <unistd.h>
#include "expendmain.proto.h"
#include "expendtable.proto.h"
#include "stor.proto.h"
#include "../morphlib/setlang.proto.h"
#include "../greeklib/isblank.proto.h"

int main(int argc, char *argv[])
{
	int c, errflg;
	char line[BUFSIZ];

	errflg = 0;
	InitGstrMem();

	while (!errflg && (c = getopt(argc,argv,"IL")) != -1) {
		switch (c) {
			case 'I':
				set_lang(ITALIAN);
				break;
			case 'L':
				set_lang(LATIN);
				break;
			default:
				break;
		}
	}


	while(fgets(line,sizeof line,stdin) ) {
		if( ! strncmp(line,":le:",4) ) {
			PrntNewGstrings(stdout,0);
			ResetGstrBuf();
			printf("\n%s", line );
			continue;
		}
		if( line[0] == ':' ) {
			printf("%s", line );
			continue;
		}
		if( is_blank(line) )
			continue;
		if( Is_comment(line) )
			continue;

		if( AddEndLine(line,"indeclform",BUFSIZ) < 0 )
			break;
	}
	PrntNewGstrings(stdout,0);
	ResetGstrBuf();
	return (0);
}

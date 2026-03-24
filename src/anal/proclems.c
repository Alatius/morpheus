#include <greek.h>
#include "../greeklib/xstrings.proto.h"
/*
 * deal with lemmas from the LSJ
 * 10/3/94 grc
 */

char firsthalf[BUFSIZ];
main()
{
	char line[BUFSIZ];
	char curlem[BUFSIZ];

	while(fgets(line, sizeof line, stdin)) {
		line[strcspn(line, "\n")] = '\0';
		if( ! line[0] || isspace(line[0]) ) continue;
		
		getlem(line,curlem);
		printf("[%s] [%s]\n", curlem, line );
	}
}


getlem(char *s, char *lem)
{
	char *savelem = lem;
	char *p;

	if(*s == '-' ) {
		Xstrncpy(lem,firsthalf,MAXWORDSIZE);
		while(*lem) lem++;
	} else
		*lem = 0;
	while(*s&&!isspace(*s)) *lem++ = *s++;
	*lem = 0;

	if((p=strchr(savelem,'-')) && *savelem != '-') {
		Xstrncpy(firsthalf,savelem,MAXWORDSIZE);
		*(strchr(firsthalf,'-')) = 0;
	}
}

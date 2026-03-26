#include <greek.h>
#include <gkstring.h>
#include <morphflags.h>
#include "../morphlib/fixacc.proto.h"
#include "../greeklib/hasaccent.proto.h"
#include "../greeklib/stripacc.proto.h"
#include "../greeklib/stripmeta.proto.h"
#include "../greeklib/xstrings.proto.h"

char half1[BUFSIZ];
static void buildw(char *targ, char *h1, char *h2);
int checkcurw(char *);
int chckdictent(char *, char *);

int main(void)
{
	char line[BUFSIZ];
	char savel[BUFSIZ];
	char curw[BUFSIZ];
	char *hp;

	while(fgets(line, sizeof line, stdin)) {
		line[strcspn(line, "\n")] = '\0';
		Xstrncpy(savel,line,sizeof(savel));
		hp = strtok(line," \t");
		if( ! hp ) continue;
		hp = strchr(line,'-');
		Xstrncpy(curw,line,sizeof(curw));
		if(hp==line) buildw(curw,half1,line);
/*sprintf(curw,"%s%s", half1, line);*/
		else if( hp) {
			Xstrncpy(half1,line,sizeof(half1));
			half1[hp-line] = 0;
			stripacc(half1);
		}
		/*else half1[0] = 0;*/
		printf("%d %s %s\n", checkcurw(curw), curw , savel);
/*
		printf("[%s] [%s]\n", curw , line );
*/

		/*printf("%d:%s\n", hp != NULL, line );*/
	}
	return 0;
}

void buildw(char *targ,char *h1, char *h2)
{
	Xstrncpy(targ, h1, BUFSIZ);
	Xstrncat(targ, h2, BUFSIZ);
	if( !hasaccent(targ)) putsimpleacc(targ);
}

int checkcurw(char*curw)
{
	char tmp[BUFSIZ];
	char keybuf[BUFSIZ];
	int rval = 0;
	
	Xstrncpy(tmp,curw,sizeof(tmp));
	keybuf[0] = 0;

	stripmetachars(tmp);

	rval = chckdictent(tmp,keybuf);
/*
 * e.g., look for words that may have multiple entries: e.g., le/gw1, le/gw2
 */
	if( !rval ) {
		Xstrncat(tmp,"1",sizeof(tmp));
		rval = chckdictent(tmp,keybuf);

	}
	return(rval);
}


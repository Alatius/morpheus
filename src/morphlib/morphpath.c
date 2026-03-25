#include <gkstring.h>

#include "morphpath.proto.h"
#include "setlang.proto.h"
static int filesopened = 0;

FILE *
 MorphFopen(char *fname, char *mode)
{
 	FILE * f;
 	char tmpname[BUFSIZ];

 	filesopened++;
 	MorphPathName(fname,tmpname);

 	if( !(f=fopen(tmpname,mode)) ) {
 		fprintf(stderr,"MorphFopen: could not open [%s]\n", tmpname );
/* just die here -- should go up higher but will live with this for now. */

/*
 		return(NULL);
*/
 	}
 	
	return(f);
}

void NumFilesOpened(void)
{
	printf("filesopened [%d]\n", filesopened );
}

/*
 * take a short pathname (e.g. "lib/vowcontr.table") and derive from it a
 * full path name (e.g. "/usr/src/local/morpheus/lib/vowcontr.table")
 */
void MorphPathName(char *shorts, char *full)
 {
 	char * s;

	s = getenv("MORPHLIB");

	if( ! s ) {
		printf("MORPHLIB not set in your environment!\n");
		return;
	}

	if( cur_lang() == LATIN )
		snprintf(full,BUFSIZ,"%s/Latin/%s", s , shorts );
	else if ( cur_lang() == ITALIAN )
		snprintf(full,BUFSIZ,"%s/Italian/%s", s , shorts );
	else
		snprintf(full,BUFSIZ,"%s/Greek/%s", s , shorts );

	/* Replace any ':' (legacy Mac path separator) with '/' */
	s=full;
	while(*s) {
		if( *s == ':' ) *s = '/';
		s++;
	}
 }

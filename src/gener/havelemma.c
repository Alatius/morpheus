#include <gkstring.h>
#include "../greeklib/binlook.proto.h"
#include "../greeklib/stripdiaer.proto.h"
#include "../greeklib/stripquant.proto.h"
#include "../greeklib/xstrings.proto.h"
#define LEMMAFILE "vlist"
static int firstshot = 1;
char ** lemmentries = NULL;
static	int nlines = 0;
static int xstrcmp(char *, char **);

int checkforlemma(char *lemmastr)
{
	int rval;
	char tmplemm[MAXWORDSIZE];
	
	if( ! lemmentries ) 
		if( ! init_lemmentries() ) return(0);
	Xstrncpy(tmplemm,lemmastr,sizeof tmplemm);
	stripquant(tmplemm);
	stripdiaer(tmplemm);

	rval = binlook( lemmentries , tmplemm , nlines , sizeof * lemmentries , 1 , xstrcmp);

	return(rval);
}

int init_lemmentries(void)
{
	FILE * flemm;
	char line[LONGSTRING], *p;
	int i;

	if( (flemm=fopen(LEMMAFILE,"r")) == NULL ) {
		if( firstshot )
			fprintf(stderr,"could not open [%s]\n", LEMMAFILE );
		firstshot = 0;
		return(0);
	}
	
	while(fgets(line,sizeof line,flemm)) {
		nlines++;
	}
	lemmentries = (char **) calloc(nlines+1,sizeof *lemmentries);
	
	fseek(flemm,0L,0);
	
	for(i=0;fgets(line,sizeof line,flemm);i++) {
		*(lemmentries+i) = (char *)calloc(strlen(line)+1,sizeof **lemmentries);
		
		p = &line[strlen(line)-1];
		while(isspace(*p)&&p>line) *p-- = 0;  /* zap newline */
		Xstrncpy(*(lemmentries+i),line,strlen(line)+1);
	}
/*
	fprintf(stderr,"%d lemmas first %s last %s\n", 
		nlines, *lemmentries , *(lemmentries+nlines-1));
*/
	fclose(flemm);
	return(1);

}

static int xstrcmp(char *p1, char **p2)
{
	int rval;
	
	rval = strcmp(p1,*p2);
/*
fprintf(stderr,"rval [%d] for [%s] and [%s]\n", rval  , *p1, *p2 );
*/
	return(rval);
}

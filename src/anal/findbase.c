#include <gkstring.h>

#include "../greeklib/issubstring.proto.h"
#include "../greeklib/stripmeta.proto.h"

extern int chckstem(char *, char *, int);

int quickflag = 0;
static void check_ew(char *p);
static void check_ih(char *p);

int main(void)
{
	char line[BUFSIZ*10];

	while(fgets(line, sizeof line, stdin)) {
		line[strcspn(line, "\n")] = '\0';
		memmove(line,line+4,strlen(line+4)+1);
		if( is_substring("ew_",line) || is_substring("aw_",line)) {
			check_ew(line);
		} else if( is_substring("i h_hs",line)) {
			check_ih(line);
		}
	}
}


void check_ew(char *p)
{
	char workstem[BUFSIZ];
	char curbase[BUFSIZ];
	char stembuf[BUFSIZ];
	char stemkeys[BUFSIZ];
	char *s;
	int rval = 0;

	strcpy(workstem,p);
	stripmetachars(workstem);
	s=workstem;
	while(*s&&!isspace(*s)) s++;
	/*if(isspace(*s) && s!=p) s--;*/
	*s = 0;

	sprintf(curbase,"%sh", workstem );
	rval = chckstem(curbase,stemkeys,1);
	if(rval) printf("%s %s", curbase,stemkeys );

	sprintf(curbase,"%sht", workstem );
	rval = chckstem(curbase,stemkeys,1);
	if(rval) printf("%s %s", curbase,stemkeys );

	sprintf(curbase,"%shs", workstem );
	rval = chckstem(curbase,stemkeys,1);
	if(rval) printf("%s %s", curbase,stemkeys );
}

void check_ih(char *p)
{
	char stemkeys[BUFSIZ*10];
	char stembuf[BUFSIZ*10];
	char *s;
	int rval;

	s = p;
	while(*s&&!isspace(*s)) s++;
	if(isspace(*s)&&s!=p) s--;
	*s = 0;
	strcpy(stembuf,p);
	stripmetachars(stembuf);
	rval = chckstem(stembuf,stemkeys,1);
	if( rval ) rval = 2;
	rval += chckstem(stembuf,stemkeys,0);
	printf("%si %d\n", p , rval);
}


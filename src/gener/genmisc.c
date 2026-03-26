#include <greek.h>
#include "../greeklib/xstrings.proto.h"

void set_baseentry(char *source, char *baseentry, int n)
{
	char * s = baseentry;
	
	Xstrncpy(baseentry,source,n);
	
	/*
	 * grab the first two words
	 */
	while(*s&&!isspace(*s)) s++;
	while(isspace(*s)) s++;
	while(*s&&!isspace(*s)) s++;
	*s = 0;
	
}

void clear_globs(char *s)
{
	while(isspace(*s)) s++;
	while(*s&&!isspace(*s)) {
		if(*s == ',' ) *s = ' ';
		s++;
	}

}

FILE * fbumwords = NULL;
FILE * foddkeys = NULL;

void SawBadForm(char *lemmastr, char *stemstr, char *stemkeys)
{
	if( ! fbumwords ) {
		if( ! (fbumwords = fopen("bumwords","w")) ) {
			return;
		}
	}
	fprintf(fbumwords,"%s\t%s\t%s\n\n", lemmastr, stemstr, stemkeys );
}

void ShowOddKeys(char *lemmastr, char *s)
{
	if( ! foddkeys ) {
		if( ! ( foddkeys = fopen("oddkeys2","w")) ) {
				return;
		}
	}
	fprintf(foddkeys,"%s\t%s\n", lemmastr, s );

}

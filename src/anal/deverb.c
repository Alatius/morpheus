#include <ctype.h>
#include <gkstring.h>

#include "../greeklib/xstrings.proto.h"
#include "../greeklib/addbreath.proto.h"
#include "../greeklib/getbreath.proto.h"
#include "../greeklib/stripquant.proto.h"
#include "../greeklib/stripstemsep.proto.h"
#include "../greeklib/stripmeta.proto.h"
#include "../morphlib/gkstring.proto.h"
#include "../morphlib/preverb2.proto.h"
#include "../morphlib/setlang.proto.h"
#include "../gkdict/derivio.proto.h"

extern int chckstem(char *, char *, int);
extern int chckvstem(char *, char *);

int quickflag = 0;
int comstemtypes1(char *,char *, char *, char *);
int comstemtypes(char *, char *, char *);
int testcmpstem(char *, char *, char *, char *, char *);
int testcmpstem2(char *, char *, char *, char *, char *);
int testcmpstem3(char *, char *, char *, char *, char *);
int checkvcomp(char *, char *, char *, char *);
int chcknstem(char *, char *);
int comNomstemtypes(char *, char *, char *, char *);
static void zaptabs(char *s);
static void firstkey(char *s);
#define VERBMATCH 1
#define DERIVMATCH 2
#define NOMMATCH 3
#define ADJMATCH 4
#define NOMMATCH2 5
#define MAMATCH 6

int main(void)
{
	char line[BUFSIZ];
	char tmp[BUFSIZ];
	char tmpstem[BUFSIZ];
	char curlemma[BUFSIZ];
	char needlemma[BUFSIZ];
	char transkeys[BUFSIZ];
	char *s;
	char *p;
	char keys[BUFSIZ];
	char reskeys[BUFSIZ];
	int rval;

	while(fgets(line, sizeof line, stdin)) {
		line[strcspn(line, "\n")] = '\0';
		transkeys[0] = keys[0] = reskeys[0] = 0;
		Xstrncpy(tmp,line,sizeof(tmp));
		s = line;

		Xstrncpy(curlemma,line,sizeof(curlemma));
		firstkey(curlemma);
		while(*s&&!isspace(*s)) s++;
		while(isspace(*s)) *s++ = 0;

		Xstrncpy(needlemma,s,sizeof(needlemma));
		firstkey(needlemma);
		while(*s&&!isspace(*s)) s++;
		while(isspace(*s)) *s++ = 0;

		while(*s&&!isspace(*s)) s++;
		while(isspace(*s)) *s++ = 0;

		p = s;

		while(*s&&!isspace(*s)) s++;
		if(*s) *s++ = 0;
		while(isspace(*s)) s++;
		Xstrncpy(keys,s,sizeof(keys));
		stripquant(p);
		stripstemsep(p);
		Xstrncpy(tmpstem,p,sizeof(tmpstem));
/*printf("curlemma [%s] needlemma [%s] tmpstem [%s] keys [%s]\n", curlemma, needlemma, tmpstem, keys );*/
		rval = testcmpstem(needlemma,tmpstem,reskeys,keys,transkeys);

		s = tmp;
		while(*s&&!isspace(*s)) s++;
		while(isspace(*s)) s++;
		
		zaptabs(line);
		zaptabs(s);
/*printf("stem [%s] keys [%s] s [%s] transkeys [%s]\n", tmpstem, keys , s ,transkeys);*/
		printf("%d\t%s\t%s\t%s\t", rval, line, s, transkeys);

		s = reskeys;
		while(*s) { 
			if(*s == ' ') putchar('\t');
			else putchar(*s);
			s++;
		}
		putchar('\n');
		
	}
}

gk_string Gstr, BlankGstr;
int testcmpstem(char *needlemma,char *stem,char * stemkeys,char* matchkeys,char * transkeys)
{
	char rawprvb[BUFSIZ], fullprevb[BUFSIZ], half2[BUFSIZ];
	char *s;
	int rval;

	rval = testcmpstem2(needlemma,stem, stemkeys, matchkeys,transkeys);
	if( rval ) return(rval);
	if( !rval ) {
		s = stem+strlen(stem)-1;
		while(s>stem) {
			Gstr = BlankGstr;
			fullprevb[0] = 0;
			Xstrncpy(half2,s,sizeof(half2));
			Xstrncpy(rawprvb,stem,sizeof(rawprvb));
			rawprvb[strlen(rawprvb) - strlen(s)] = 0;
			set_gkstring(&Gstr,half2);
			if( is_preverb(rawprvb,fullprevb,&Gstr) ) {
				char tmp[BUFSIZ];
				tmp[0] = 0;

				SprintGkFlags(&Gstr,tmp,sizeof(tmp),":",1);
				if(testcmpstem2(needlemma,half2, stemkeys, matchkeys,transkeys)) { 
					Xstrncat(transkeys,":pb:",sizeof(transkeys));
					Xstrncat(transkeys,fullprevb,sizeof(transkeys));
					Xstrncat(stemkeys,tmp,sizeof(stemkeys));
					return(DERIVMATCH);
				}
			}

			s--;
		}
	}
	return(0);
}

int testcmpstem2(char *needlemma,char *stem,char * stemkeys,char* matchkeys,char * transkeys)
{
	int rval;

	rval=testcmpstem3(needlemma,stem, stemkeys, matchkeys,transkeys);
/*
 * grc 9/7/94
 *
 * you get metrical lengthening in poetry: e.g. 
 *	dus-ech/nustos for dus-ec-a/nustos
 * check for this.
 *
 */
	if( ! rval ) {
		char tmpstem[BUFSIZ];
		if( *stem == 'h' ) {
			Xstrncpy(tmpstem,stem,sizeof(tmpstem));
			tmpstem[0] = 'e';
			rval=testcmpstem3(needlemma,tmpstem, stemkeys, matchkeys,transkeys);
			if(rval) {
				Xstrncat(transkeys,":metrical_long:poetic",sizeof(transkeys));
				return(rval);
			}
			tmpstem[0] = 'a';
			rval=testcmpstem3(needlemma,tmpstem, stemkeys, matchkeys,transkeys);
			if(rval) {
				Xstrncat(transkeys,":metrical_long:poetic",sizeof(transkeys));
				return(rval);
			}
		} else if(*stem == 'w' ) {
		}
	}
	return(rval);
}

int testcmpstem3(char*needlemma,char *stem,char * stemkeys,char* matchkeys,char*transkeys)
{
	(void)transkeys;
	char savestem[BUFSIZ], tmp[BUFSIZ];
	int rval = 0;
	
	stemkeys[0] = 0;

        if( *stem == 'r'  && getbreath(stem) == NOBREATH ) {

		if( *(stem+1) == 'r' ) memmove(stem,stem+1,strlen(stem+1)+1); /* rr --> r */
                Xstrncpy(tmp,"r(",(int)sizeof tmp);
                Xstrncat(tmp,stem+1,(int)sizeof tmp);
                Xstrncpy(stem,tmp,BUFSIZ);
                rval = checkvcomp(needlemma,stem,stemkeys,matchkeys);
		if( rval ) return(rval);
		return(0);
        }

        if( Is_vowel(*stem) && getbreath(stem) == NOBREATH  && cur_lang() != LATIN ) {

                Xstrncpy(savestem,stem,sizeof(savestem));
/*
 * check for rough breathing
 */

                addbreath(stem,ROUGHBR);
		rval = checkvcomp(needlemma,stem, stemkeys,matchkeys);
		if( rval ) return(rval);

		Xstrncpy(stem,savestem,BUFSIZ);
                addbreath(stem,SMOOTHBR);
		rval = checkvcomp(needlemma,stem, stemkeys,matchkeys);
		if( rval ) return(rval);
		Xstrncpy(stem,savestem,BUFSIZ);
	}
	rval = checkvcomp(needlemma,stem, stemkeys,matchkeys);
	/*if( rval ) printf("%d:[%s] [%s] [%s]\n", rval, stem, stemkeys, matchkeys);*/
	return(rval);
}

int checkvcomp(char * needlemma,char * stem,char * stemkeys,char * matchkeys)
{
	int rval = 0;
	char mbuf[BUFSIZ];


	*stemkeys = 0;
	Xstrncpy(mbuf,matchkeys,sizeof(mbuf));

	rval = chcknstem(stem,stemkeys);
	if( rval ) rval = comNomstemtypes(needlemma,stem,stemkeys,matchkeys);
	if( rval ) return(rval);
	Xstrncpy(matchkeys,mbuf,BUFSIZ);

	*stemkeys = 0;
	rval = chckvstem(stem,stemkeys);
	if( rval ) rval = comstemtypes1(needlemma,stem,stemkeys,matchkeys);
	if( rval ) return(NOMMATCH);
	Xstrncpy(matchkeys,mbuf,BUFSIZ);

	*stemkeys = 0;
	if( ! rval ) rval = checkforderiv(stem, stemkeys);
/*printf("rval:%d stem [%s] [%s] [%s]\n", rval , stem, stemkeys, matchkeys );*/
	if( rval ) rval = comstemtypes1(needlemma,stem,stemkeys,matchkeys);
	if( rval ) return(NOMMATCH);
	stemkeys[0] = 0;
	Xstrncpy(matchkeys,mbuf,BUFSIZ);

	return(0);
}

int chcknstem(char * stem,char *stemkeys)
{
	int rval;
	* stemkeys = 0;
	rval = chckstem(stem,stemkeys,1);
	return(rval);
}


int comNomstemtypes(char * needlemma,char * stem,char * stemkeys,char * matchkeys)
{
	char mbuf[BUFSIZ];
	char tmpstem[BUFSIZ];
	int rval = 0;

	tmpstem[0] = 0;
	Xstrncpy(mbuf,matchkeys,sizeof(mbuf));
	Xstrncpy(tmpstem,stemkeys,sizeof(tmpstem));
	rval = comstemtypes1(needlemma,stem,stemkeys,mbuf);
	if( rval ) return( NOMMATCH );

/*printf("stem [%s] stemkeys [%s] mbuf [%s]\n", stem,stemkeys,mbuf );*/
	if( !strncmp("os_h_on",matchkeys,strlen("os_h_on"))) {
		Xstrncpy(stemkeys,tmpstem,BUFSIZ);
		Xstrncpy(mbuf,"os_ou",sizeof(mbuf));
		rval = comstemtypes1(needlemma,stem,stemkeys,mbuf);
		if(rval) return(NOMMATCH);
	}
	if( !strncmp("os_on",matchkeys,strlen("os_on"))) {
		Xstrncpy(stemkeys,tmpstem,BUFSIZ);
		Xstrncpy(mbuf,"h_hs os_ou a_hs",sizeof(mbuf));
		rval = comstemtypes1(needlemma,stem,stemkeys,mbuf);
		if(rval) return(NOMMATCH);

		Xstrncpy(stemkeys,tmpstem,BUFSIZ);
		Xstrncpy(mbuf,"os_h_on",sizeof(mbuf));
		rval = comstemtypes1(needlemma,stem,stemkeys,mbuf);
		if(rval) return(ADJMATCH);

		Xstrncpy(stemkeys,tmpstem,BUFSIZ);
		Xstrncpy(mbuf,"hs_ou",sizeof(mbuf));
		rval = comstemtypes1(needlemma,stem,stemkeys,mbuf);
		if(rval) return(NOMMATCH);

		Xstrncpy(stemkeys,tmpstem,BUFSIZ);
		Xstrncpy(mbuf,"hs_eos",sizeof(mbuf));
		rval = comstemtypes1(needlemma,stem,stemkeys,mbuf);
		if(rval) return(NOMMATCH);

		if( stem[strlen(stem)-1] == 'm' ) {
			char tmps[BUFSIZ];
			
			Xstrncpy(tmps,stem,sizeof(tmps));
			Xstrncpy(stemkeys,tmpstem,BUFSIZ);
			tmps[strlen(tmps)-1] = 0;
			Xstrncpy(mbuf,"ma_matos",sizeof(mbuf));
			rval = comstemtypes1(needlemma,tmps,stemkeys,mbuf);
			if(rval) return(MAMATCH);
		}

	}
	if( !strncmp("os_h_on",matchkeys,strlen("os_h_on"))) {
		Xstrncpy(stemkeys,tmpstem,BUFSIZ);
		Xstrncpy(mbuf,"os_on",sizeof(mbuf));
		rval = comstemtypes1(needlemma,stem,stemkeys,mbuf);
		if(rval) return(ADJMATCH);

		Xstrncpy(stemkeys,tmpstem,BUFSIZ);
	}
	if( !strncmp("hs_es",matchkeys,strlen("hs_es"))) {
		Xstrncpy(stemkeys,tmpstem,BUFSIZ);
		Xstrncpy(mbuf,"hs_eos os_ou h_hs a_hs ws_oos",sizeof(mbuf));
		rval = comstemtypes1(needlemma,stem,stemkeys,mbuf);
		if(rval) return(NOMMATCH2);

		Xstrncpy(stemkeys,tmpstem,BUFSIZ);
	}
	if( !strncmp("wn_on",matchkeys,strlen("wn_on"))) {
		Xstrncpy(stemkeys,tmpstem,BUFSIZ);
		Xstrncpy(mbuf,"wn_onos",sizeof(mbuf));
		rval = comstemtypes1(needlemma,stem,stemkeys,mbuf);
		if(rval) return(NOMMATCH2);

		Xstrncpy(stemkeys,tmpstem,BUFSIZ);
	}
	if( !strncmp("is_idos",matchkeys,strlen("is_idos"))) {
		Xstrncpy(stemkeys,tmpstem,BUFSIZ);
	/*
	 * grc 9/7/94
	 * note: this allows us to go from is_idos_adj --> is_idos
 	 * as in dusmhn is_idos_adj --> mhn is_idos
	 */
		Xstrncpy(mbuf,"is_idos is_ews",sizeof(mbuf));
		rval = comstemtypes1(needlemma,stem,stemkeys,mbuf);
		if(rval) return(NOMMATCH2);

		Xstrncpy(stemkeys,tmpstem,BUFSIZ);
		Xstrncpy(mbuf,"h_hs",sizeof(mbuf));
		rval = comstemtypes1(needlemma,stem,stemkeys,mbuf);
		if(rval) return(NOMMATCH2);

		Xstrncpy(stemkeys,tmpstem,BUFSIZ);
	}
	if( !strncmp("us_eia_u",matchkeys,strlen("us_eia_u"))) {
		Xstrncpy(stemkeys,tmpstem,BUFSIZ);
		Xstrncpy(mbuf,"us_uos uLs_uos us_ews",sizeof(mbuf));
		rval = comstemtypes1(needlemma,stem,stemkeys,mbuf);
		if(rval) return(NOMMATCH2);

		Xstrncpy(stemkeys,tmpstem,BUFSIZ);
	}
		
	if( !strncmp("oos_oon",matchkeys,strlen("oos_oon"))) {
		Xstrncpy(stemkeys,tmpstem,BUFSIZ);
		Xstrncpy(mbuf,"oos_oou",sizeof(mbuf));
		rval = comstemtypes1(needlemma,stem,stemkeys,mbuf);
		if(rval) return(NOMMATCH2);

		Xstrncpy(stemkeys,tmpstem,BUFSIZ);
	}
		
	return(0);
}

void zaptabs(char *s)
{
	while(*s) {
		if(*s=='\t') *s =' ';
		s++;
	}
}

int comstemtypes1(char *needlemma,char *stem,char *stemkeys,char *matchkeys)
{
	char tmp[BUFSIZ];
	int rval;

	rval = comstemtypes(stem,stemkeys,matchkeys);
if(*stemkeys)
printf("needlemma [%s] stemkeys [%s] match [%s]\n", needlemma, stemkeys , matchkeys);
	if(rval && *needlemma) {
		char * sp;
		char curbuf[BUFSIZ];
		Xstrncpy(tmp,needlemma,sizeof(tmp));
		Xstrncat(tmp,":",sizeof(tmp));

		curbuf[0] = 0;
		sp = stemkeys;
		while(*sp) {	

			if( !strncmp(tmp,sp,strlen(tmp)) ) {
				char * t = curbuf;
				if(curbuf[0]) Xstrncat(curbuf," ",sizeof(curbuf));
				Xstrncat(curbuf,sp,sizeof(curbuf));
				while(*t&&!isspace(*t))t++;
				*t = 0;
				sp = curbuf;
			}
			while(*sp&&!isspace(*sp)) sp++;
			while(isspace(*sp)) sp++;
		}
		if( !curbuf[0] ) {
			return(0);
		}
		Xstrncpy(stemkeys,curbuf,BUFSIZ);
	}
	return(rval);
}

void firstkey(char *s)
{
	while(*s&&!isspace(*s)) s++;
	if(isspace(*s)) *s=0;
}

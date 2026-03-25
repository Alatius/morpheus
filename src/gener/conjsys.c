#include <gkstring.h>
#include <string.h>
#include "../morphlib/beta2rtf.proto.h"
#include "../morphlib/gkstring.proto.h"
#include "../morphlib/morphflags.proto.h"
#include "../morphlib/nextkey.proto.h"
#include "../gkends/retrends.proto.h"
#include "../greeklib/xstrings.proto.h"
#include "combconj.proto.h"
/* #define FULLKEYS "pr fu ao pf pp ap"*/
int irreg_conj(void);
int regular_entry(char *);
int noppart(char *);
int is_empty(char *);
int need_ppart(char *);
int check_vsdupl(char *, FILE *);
int need_codupl(char *);
int has_alpha(char *);

#define DEFPPARTS 1
char *fullkeys[] = {
		"pr",	
		"fu",
		"ao",
		"pf",
		"pp",
		"ap",
		"fp",
		"p4" /* latin 4th principle part */
		"va" /* verbal adjective */
		"vn" /* verbal noun */
		};

void dummyfnc(void)
{
}

static char curlemma[MAXWORDSIZE*2];
static	char linebuf[LONGSTRING];
static	char origline[LONGSTRING];
static	char stembuf[MAXWORDSIZE];
static	char derivbuf[LONGSTRING];
static	char savekeys[LONGSTRING];
static	char globalkeys[LONGSTRING];
static	char vsbuf[LONGSTRING];
static	char cobuf[LONGSTRING];
static int npparts = -1;
static int wantpparts = 0;

int fullconj = 0;

static void show_defvals(FILE *fout);
static void set_newlemma(char *s);

void GenConjForms(FILE *fin, FILE *fout, int conjmode)
{
	char * lp;
	char saveline[LONGSTRING];
	
	fullconj = conjmode;
	
	while(fgets(linebuf,sizeof linebuf,fin) ) {
		Xstrncpy(saveline,linebuf,sizeof saveline);
		if( !strncmp(linebuf,"@fullconj",strlen("@fullconj")) )  {
			fullconj = 1;
			continue;
		}
		if( ! strncmp(linebuf,"@shortconj",strlen("@shortconj")) ) {
			fullconj = 0;
			continue;
		}
		
		if( ! strncmp(linebuf,":le:",4) ) {

			if( ! npparts && wantpparts && (irreg_conj() || fullconj)) {
				show_defvals(fout);
				fprintf(fout,"\n");
			}
			vsbuf[0] = cobuf[0] = 0;
			npparts = 0;
		 	wantpparts = 0;
			fprintf(fout,"%s", linebuf);
			set_newlemma(linebuf);
			continue;
		}
			
		if( has_pref(linebuf,":vs:" ) ) {
		 	wantpparts = 0;
		 	derivbuf[0] = 0;
		 	Xstrncpy(vsbuf,linebuf,sizeof vsbuf);
		 	fprintf(fout,"%s", linebuf );
		 	continue;
		}
		
		if( has_pref(linebuf,":aj:") 
		 || has_pref(linebuf,":no:") ) {
		 	wantpparts = 0;
		 	derivbuf[0] = 0;
		 	fprintf(fout,"%s", linebuf );
		 	continue;
		}
		
		if( ! strncmp(":de:",linebuf,4) ) {
			if( ! npparts && wantpparts && (irreg_conj() || fullconj)) {
				show_defvals(fout);
				fprintf(fout,"\n");
			}
			npparts = 0;
			wantpparts = 1;
			if( linebuf[strlen(linebuf)-1] == '\n' )
					linebuf[strlen(linebuf)-1] = 0;
			Xstrncpy(origline,linebuf,sizeof origline);
			nextkey(linebuf,stembuf);
			memmove(stembuf,stembuf+4,strlen(stembuf+4)+1);
			nextkey(linebuf,globalkeys);
			/*
			 * keys that are attached directly to the derivstr are inherited by
			 * all subsequent forms
			 */
			 lp = globalkeys;
			 while(*lp) {
			 	if(*lp == ',') *lp = ' ';
			 	lp++;
			 }

			 nextkey(globalkeys,derivbuf);
			 if( * globalkeys ) {
			 	Xstrncat(globalkeys," ",sizeof globalkeys);
			 }
			 Xstrncat(globalkeys, linebuf,sizeof globalkeys);
			if( ! fullconj && ! regular_entry(derivbuf) )
				fprintf(fout,"-");
			fprintf(fout,"%s", saveline );

			if(noppart(linebuf)){
	 /*
	  * if you have a form such as ":de:a)gaqoerg ew_denom",
	  * with no specific principle parts mentioned, then we
	  * assume that all principle parts are possible
	  */
				char tkeys[256];
				snprintf(tkeys,sizeof(tkeys),"%s %s", fullkeys[0] , linebuf );
/*
				if( ! combine_conj(fout,curlemma,origline,stembuf,derivbuf,globalkeys,tkeys) )
					fprintf(fout,"error1: nothing found\n");
*/
/*
				for(i=0;i<sizeof fullkeys/sizeof fullkeys[0];i++) {
					char tkeys[256];
					snprintf(tkeys,sizeof(tkeys),"%s %s", fullkeys[i] , linebuf );
					combine_conj(fout,curlemma,origline,stembuf,derivbuf,globalkeys,tkeys);

					printf("stembuf [%s] global [%s] deriv [%s] tk [%s]\n", 
					stembuf , globalkeys, derivbuf, tkeys );
				}
*/
			} else {
				if( ! combine_conj(fout,curlemma,origline,stembuf,derivbuf,globalkeys,linebuf) ) {
					fprintf(fout,"error2: nothing found\n");
				}

/*
				printf("stembuf [%s] deriv [%s] curk [%s]\n", stembuf , derivbuf, linebuf );
*/
			}
			continue;
			
		}
		if( linebuf[0] == ';' ) {
				char tkeys[256];
				int len;
				int rval;
				char *s;
				
				fprintf(fout,"%s", linebuf );
				Xstrncpy(cobuf,linebuf,sizeof cobuf);
				s = cobuf;
				while(*s&&!isspace(*s)) s++;
				while(isspace(*s))s++;
				while(*s&&!isspace(*s)) s++;
				*s = 0;
				
				len = strlen(linebuf);
				if( linebuf[len-1] == '\n') linebuf[len-1] = 0;

				Xstrncpy(tkeys, linebuf+1, sizeof tkeys );
				Xstrncpy(savekeys,linebuf+1,sizeof savekeys);
				s = savekeys; 
				while(*s&&!isspace(*s)) s++; 
				if( isspace(*s) ) *(s++) = 0;

/*
printf("tkeys [%s] needpp %d\n", tkeys, need_ppart(tkeys ) );
*/
				if( ! need_ppart(tkeys) && has_alpha(stembuf) ) {
					continue;
				}
				
				npparts++;
				rval = combine_conj(fout,curlemma,origline,stembuf,derivbuf,globalkeys,tkeys);
				if( ! rval ) {
					fprintf(fout,"error4: nothing found\n");
printf("rval %d stembuf [%s] global [%s] deriv [%s] tk [%s]\n", rval,
					stembuf , globalkeys, derivbuf, tkeys );
}
				continue;
		}
		if( linebuf[0] == '@') {
				char tkeys[256];
				int len;
				int rval;
				
				if( ! derivbuf[0] ) {
					if( ! fullconj ) {
						check_vsdupl(linebuf,fout);
					} else
						fprintf(fout,"%s", linebuf );
					continue;
				} 
				npparts++;
				fprintf(fout,"-%s", linebuf );

				if( ! need_codupl(linebuf) && has_alpha(stembuf) ) continue;
				
				len = strlen(linebuf);
				if( linebuf[len-1] == '\n') linebuf[len-1] = 0;

				snprintf(tkeys, sizeof(tkeys), "%s %s", savekeys, linebuf+1 );
				if( ! need_ppart(tkeys)  ) 
					continue;
				rval = combine_conj(fout,curlemma,origline,stembuf,derivbuf,globalkeys,tkeys);
				if( ! rval )
					fprintf(fout,"error5: nothing found\n");

				continue;
		}
		
		fprintf(fout,"%s", linebuf );

	}
	if( ! npparts && wantpparts && (irreg_conj() || fullconj ) ) 
			show_defvals(fout);


}

/* TODO: callers formerly passed a count argument (fullconj ? DEFPPARTS : 1)
 * that was silently ignored due to K&R calling conventions.
 * Consider adding an nparts parameter instead of hardcoding DEFPPARTS. */
void show_defvals(FILE *fout)
{
	int i;

	for(i=0;i< DEFPPARTS;i++) {
		char tkeys[256];
		snprintf(tkeys,sizeof(tkeys),"%s ", fullkeys[i]  );
		combine_conj(fout,curlemma,origline,stembuf,derivbuf,globalkeys,tkeys);
/*
		printf("stembuf [%s] global [%s] deriv [%s] tk [%s]\n", 
		stembuf , globalkeys, derivbuf, tkeys );
*/
	}

}
void set_newlemma(char *s)
{
	*(s+strlen(s)-1) = 0;
	Xstrncpy(curlemma,s+4,sizeof curlemma);
	derivbuf[0] = 0;
}

int noppart(char *s)
{
	char buf1[256];
	char buf2[256];
	size_t i;

	if(is_empty(s) ) return(1);
	
	Xstrncpy(buf1,s,sizeof buf1);
	nextkey(buf1,buf2);
	
	if( ! buf2[0] ) return(1);
	
	if(buf2[2] && buf2[2] != ',' && ! isspace(buf2[2]) ) {
		return(1);
	}
	for(i=0;i<sizeof fullkeys/sizeof fullkeys[0];i++)
		if( ! strncmp(fullkeys[i],buf2,2) ) {
			return(0);	
		}
	return(1);

}

int is_empty(char *s)
{
	while(*s) {
		if( !isspace(*s++) ) return(0);
	}
	return(1);
}

int has_pref(char *s, char *pref)
{
	return(strncmp(s,pref,strlen(pref)) == 0 );
}

static gk_string   BlnkGstr;
static gk_word BlnkGkword;
int need_ppart(char *s)
{
	gk_string  GlobGstr, CurGstr;
	gk_word TmpGkword;
	char tmpkeys[LONGSTRING];
	char tmpglobs[LONGSTRING];
	int rval;
	
	if( fullconj ) 
		return(1);
		
	while(*s&&!isspace(*s)&&*s!=',') s++;
	if(*s == ',' ) {
		s++;
		if(*s == '-' ) { /* skip attached ending */
			while(*s&&*s!=','&&!isspace(*s)) s++;
			if(*s ==',') s++;
		}
	}
	Xstrncpy(tmpkeys,s,sizeof tmpkeys);
	s = tmpkeys;
	
	while(*s&&!isspace(*s)) {
		if(*s == ',' ) *s = ' ';
		s++;
	}
	
	snprintf(tmpglobs,sizeof(tmpglobs),"%s %s\n", derivbuf , globalkeys );
	GlobGstr = CurGstr = BlnkGstr;
	TmpGkword = BlnkGkword;
	ScanAsciiKeys(tmpglobs,&TmpGkword,&GlobGstr,NULL);
/*
printf("\ntmpglobs [%s] dtype [%o] reg %o\n", tmpglobs , derivtype_of(&GlobGstr), Is_regconj(&GlobGstr));
*/
/*
 * always generate forms from rough types such as reg_conj
 */

	if( ! Is_regconj(&GlobGstr) ) return(1);
		
	if( has_morphflag(morphflags_of(&GlobGstr),N_INFIX) ||
	    has_morphflag(morphflags_of(&GlobGstr),NO_REDUPL ) ||	
		has_morphflag(morphflags_of(&GlobGstr),PRES_REDUPL) ) {
			return(1);
	}

	TmpGkword = BlnkGkword;
	ScanAsciiKeys(tmpkeys,&TmpGkword,&CurGstr,NULL);
	if( has_morphflag(morphflags_of(&CurGstr),N_INFIX) ||
	    has_morphflag(morphflags_of(&CurGstr),NO_REDUPL ) ||	
	    has_morphflag(morphflags_of(&CurGstr),SYLL_AUG ) ||	
		has_morphflag(morphflags_of(&CurGstr),PRES_REDUPL) ) {
			return(1);
	}
	rval = WantGkEnd(&GlobGstr,&CurGstr,NO,NO);
/*
printf("savekeys:%s\nglobs:%s\nrval %d\n", tmpkeys , globalkeys , rval);
*/
	return( rval <= 0 );
}

int check_vsdupl(char *s, FILE *fout)
{
	gk_string  GlobGstr, CurGstr;
	gk_word TmpGkword;
	char tmpvsbuf1[LONGSTRING];
	char tmpvsbuf2[LONGSTRING];
	char tmpglobs[LONGSTRING];
	int rval = 0;
	char *p;
	
	GlobGstr = CurGstr = BlnkGstr;
	TmpGkword = BlnkGkword;

	fprintf(fout,"-%s", s );
	if( fullconj ) return(1);
	
	Xstrncpy(tmpglobs,vsbuf,sizeof tmpglobs);
	p = tmpglobs;
	while(*p&&!isspace(*p)) p++;
	while(isspace(*p)) p++;
	while(*p&&!isspace(*p) ){
		if(*p==',') *p = ' ';
		p++;
	}
	Xstrncpy(tmpvsbuf1,tmpglobs,sizeof tmpvsbuf1);
	*p = 0;
	
	ScanAsciiKeys(s+1,&TmpGkword,&CurGstr,NULL);
	if( has_morphflag(morphflags_of(&CurGstr),SYLL_AUG ) )
		rval = -1;
	
	snprintf(tmpvsbuf2,sizeof(tmpvsbuf2),"%s%s", tmpglobs,s+1);
	
	p = tmpvsbuf1; while(*p && !isspace(*p)) p++;
	ScanAsciiKeys(p,&TmpGkword,&GlobGstr,NULL);

	p = tmpvsbuf2; while(*p && !isspace(*p)) p++;
	ScanAsciiKeys(p,&TmpGkword,&CurGstr,NULL);
/*
	if( has_morphflag(morphflags_of(&CurGstr),SYLL_AUG ) )
		rval = -1;
*/
	if( ! rval ) 
		rval = WantGkEnd(&GlobGstr,&CurGstr,NO,YES);

			if( rval <= 0 ) {
		fprintf(fout,"%s", tmpvsbuf2 );
		if( *(lastn(tmpvsbuf2,1)) != '\n' )
			fprintf(fout,"\n");
	}
	return(rval<=0);
}

int need_codupl(char *s)
{
	gk_string  GlobGstr, CurGstr;
	gk_word TmpGkword;
	char tmpcobuf1[LONGSTRING];
	char tmpcobuf2[LONGSTRING];
	char tmpglobs[LONGSTRING];
	int rval;
	char *p;
	
	GlobGstr = CurGstr = BlnkGstr;
	TmpGkword = BlnkGkword;

/*
	fprintf(fout,"-%s", s );
*/
	if( fullconj ) return(1);
	
	Xstrncpy(tmpglobs,cobuf,sizeof tmpglobs);
	p = tmpglobs;
	while(*p&&!isspace(*p)) p++;
	while(isspace(*p)) p++;
	while(*p&&!isspace(*p) ){
		if(*p==',') *p = ' ';
		p++;
	}
	Xstrncpy(tmpcobuf1,tmpglobs,sizeof tmpcobuf1);
	*p = 0;
	snprintf(tmpcobuf2,sizeof(tmpcobuf2),"%s%s", tmpglobs,s+1);
	
	p = tmpcobuf1; while(*p && !isspace(*p)&& *p !=',' ) p++;
	if(*p == ',' ) p++;
	ScanAsciiKeys(p,&TmpGkword,&GlobGstr,NULL);

	p = tmpcobuf2; while(*p && !isspace(*p)&& *p !=',' ) p++;
	if(*p == ',' ) p++;
	ScanAsciiKeys(p,&TmpGkword,&CurGstr,NULL);
	if( has_morphflag(morphflags_of(&CurGstr),SYLL_AUG ) ||
		has_morphflag(morphflags_of(&CurGstr),N_INFIX) ||
		has_morphflag(morphflags_of(&CurGstr),NO_REDUPL ))
		return(1);
		
	rval = WantGkEnd(&GlobGstr,&CurGstr,NO,YES);
/*

printf("b1 [%s] b2 [%s] rval %d\n", tmpcobuf1, tmpcobuf2 , rval );		
	if( rval <= 0 ) {
		fprintf(fout,"%s", tmpcobuf2 );
		if( *(lastn(tmpcobuf2,1)) != '\n' )
			fprintf(fout,"\n");
	}
*/
	return(rval<=0);
}

int regular_entry(char *s)
{
	gk_string * gstr;
	gk_word * gkw;
	int rconj;
	
	gstr = CreatGkString(1);
	gkw = CreatGkword(1);
	
	ScanAsciiKeys(s,gkw,gstr,NULL);
	
	rconj = Is_regconj(gstr);
	FreeGkString(gstr);
	FreeGkword(gkw);
	return(rconj);
}

int has_alpha(char *s)
{

	while(*s) {
		if(isalpha(*s) ) return(1);
		s++;
	}
	return(0);
}

int irreg_conj()
{
	gk_word TmpGkword;
	gk_string GlobGstr;
	GlobGstr = BlnkGstr;
	TmpGkword = BlnkGkword;
	
	ScanAsciiKeys(derivbuf,&TmpGkword,&GlobGstr,NULL);

	if( ! Is_regconj(&GlobGstr) ) return(1);
	return(0);
}

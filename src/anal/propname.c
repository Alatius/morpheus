#include <stdlib.h>
#include <ctype.h>
#include <gkstring.h>

#include "propname.proto.h"
#include "checkdict.proto.h"
#include "checkstring.proto.h"
#include "../greeklib/xstrings.proto.h"
#include "../greeklib/stripacc.proto.h"
#include "../greeklib/stripdiaer.proto.h"
#include "../greeklib/stripbreath.proto.h"
#include "../greeklib/getbreath.proto.h"
#include "../greeklib/subchar.proto.h"
#include "../morphlib/gkstring.proto.h"
#include "../morphlib/nextkey.proto.h"
#include "../morphlib/trimwhite.proto.h"

#include "prntanal.proto.h"

extern int chcknend(char *, char *);
int quickflag = 0;
long prevmemory = 0;
FILE * fpnames = NULL;
char tbuf[5000], possbuf[10000];

int main(void)
{
	FILE * finput, *foutput, *fnew;
	char line[BUFSIZ*4];
	char fname[BUFSIZ],inpname[BUFSIZ], outname[BUFSIZ];
	char newname[BUFSIZ];
	
	int rval;
	long nwords = 0;
	long nhits = 0;
	char * p;
	fname[0] = 0;

	fprintf(stderr,"use stdout? ");
	fgets(line, sizeof line, stdin);
	line[strcspn(line, "\n")] = '\0';

	if(line[0] == 'y' ) {
		fprintf(stderr,"type in forms\n");
		Xstrncpy(outname,"out.morph",sizeof(outname));
		finput = stdin;
	} else {
		fprintf(stderr,"word file? ");
		fgets(fname, sizeof fname, stdin);
		fname[strcspn(fname, "\n")] = '\0';
		
		Xstrncpy(inpname,fname,sizeof(inpname));
		Xstrncat(inpname,".words",sizeof(inpname));
		
		Xstrncpy(outname,fname,sizeof(outname));
		Xstrncat(outname,".morph",sizeof(outname));

		Xstrncpy(newname,fname,sizeof(newname));
		Xstrncat(newname,".nstem",sizeof(newname));

		if((finput=fopen(inpname,"r")) == NULL) {
			fprintf(stderr,"cannot find [%s]!\n", inpname);
			exit(-1);
		}

		if((fnew=fopen(newname,"w")) == NULL) {
			fprintf(stderr,"cannot create [%s]!\n", newname);
			exit(-1);
		}

	
	}
			
	if( ! fpnames ) {
			char tmp[MAXWORDSIZE];
			
			if( ! fname[0] ) {
				printf("basename? ");
				fgets(fname, sizeof fname, stdin);
				fname[strcspn(fname, "\n")] = '\0';
			}
			Xstrncpy(tmp,fname,sizeof(tmp));
			Xstrncat(tmp,".enames",sizeof(tmp));
			fpnames = fopen(tmp,"r");
			if( !fpnames ) {
				fprintf(stderr,"%s not found!\n", tmp);
				exit(-1);
			}
		}
	

	if( (foutput=fopen(outname,"w")) == NULL) {
		fprintf(stderr,"cannot find [%s]!\n", outname);
		exit(-1);
	}

	while(fgets(line,sizeof line,finput)) {
/*
		printf("%s %s %d\n", line, prevl , dictstrcmp(line,prevl) );
		strcpy(prevl,line);
		continue;
*/		
		trimwhite(line);
		trimdigit(line);
		p = line;
		while(*p&&!isspace(*p)) p++;
		if(*p) *p = 0;
		if( isspace(line[0]) ) continue;
		if( line[2] == '*' ) continue;
		rval = checkstring(line,(PrntFlags)SHOW_LEMMA|STRICT_CASE,foutput);
/*
		if( rval ) 
			printf("%s",anal_buf() );
*/
		if( ! rval ) 
			rval = checkpropname(line,fnew);
		nwords++;
		if( rval ) {
			nhits++;
		} else {
/*
			printf(":failed %s\n", line );
*/
			fprintf(foutput,":failed %s\n", line );
			fflush(foutput);
		}
/*
freemem = (long) FreeMem();
fprintf(stderr,"%ld %ld %0.2f %s %d %ld %ld \n", 
nwords , nhits, 100* ((float)nhits/(float)nwords) , line , rval , freemem , prevmemory - freemem );
		prevmemory = freemem;
*/
if( ! (nwords % 50 ) ) 
fprintf(stderr,"%ld %ld %0.2f %s %d\n", 
nwords , nhits, 100* ((float)nhits/(float)nwords) , line , rval  );
	}
	if( finput != stdin )
		fclose(finput);
	fclose(foutput);
}

void trimdigit(char *s)
{
	char * p = s;
	while(*s) s++;
	s--;
	while(isdigit(*s)&&s>p) *s-- = 0;
}

int checkpropname(char *s, FILE *f)
{
	char tmpname[BUFSIZ], *wp, curend[BUFSIZ], endkeys[BUFSIZ];
	char half1[BUFSIZ];
	int is_ending = 0;
	char xlit[BUFSIZ];
	int rval = 0;
	
	Xstrncpy(tmpname,s,sizeof(tmpname));
	endkeys[0] = 0;
/*
printf("%s\n", s);	
*/
	wp = tmpname;
	while( *wp ) {

	    Xstrncpy(curend,wp,sizeof(curend));
	    stripacc(curend);
	    stripdiaer(curend);
	    Xstrncpy(half1,tmpname,sizeof half1);
	    half1[wp-tmpname]=0;
	    wp++;

		is_ending = chcknend(curend,endkeys);
		if( is_ending ) {
			Xliterate(half1,xlit/*,curend*/);
/*
			printf("%s\t%s\t%s\t%s\n", xlit, half1, curend, endkeys );
*/
			rval += tryxlits(f,xlit,endkeys,s,half1,curend);
		}

	}

	return(rval);
}

int tryxlits(FILE *f, char *xlit, char *keys, char *fulls, char *half1, char *ends)
{
	char curkey[BUFSIZ];
	char workkey[BUFSIZ];
	int rval = 0;
	
	Xstrncpy(workkey,keys,sizeof(workkey));
	
	possbuf[0] = 0;
	while(nextkey(workkey,curkey) ) {
		rval += checkposs(f,xlit,curkey,fulls,half1,ends);
	}
	if(rval ) fprintf(f,"%s", possbuf);
	fflush(f);
	return(rval);
}

void Xliterate(char *s1, char *s2)
{
	char tmp[BUFSIZ];
	int add_h = 0;

	Xstrncpy(tmp,s1,sizeof(tmp));
	if( getbreath(tmp) == ROUGHBR) {
		add_h = 1;
	}

	Xstrncpy(tmp,s1+1,sizeof(tmp));
	stripbreath(tmp);
	stripacc(tmp);
	stripdiaer(tmp);
	reliterate(tmp);

	if( add_h ) {
		if( tmp[0] == 'r' ) {
			Xstrncpy(s2,"rh",MAXWORDSIZE);
			Xstrncat(s2,tmp+1,MAXWORDSIZE);
		} else {
			Xstrncpy(s2,"h",MAXWORDSIZE);
			Xstrncat(s2,tmp,MAXWORDSIZE);
		}
	} else
		Xstrncpy(s2,tmp,MAXWORDSIZE);
	*s2 = toupper(*s2);
}


typedef struct {
	char stype[20];
	char suffix[20];
	char addsuff[20];
	char keys[30];
} nom_tab;

nom_tab Nom_tab[] = {
	"s_tos",		"s",	"",		" ",
	"n_ntos",		"n",	"",		"masc ",
	"n_nos",		"n",	"",		"masc ",
	"aos_aou",		"aus",	"",		"masc ",
	"us_uos",		"ys",	"",		" ",
	"wr_oros",		"or",	"",		"masc ",
	"c_ktos",		"x",	"",		"masc ",
	"c_kos",		"x",	"",		"masc ",
	"y_pos",		"ps",	"",		"masc ",
	"wn_ontos",		"on",	"",		"masc ",
	"wn_onos",		"on",	"",		"masc ",
	"wn_onos",		"ones",	"",		"masc pl",
	"w_oos",		"o",	"",		"fem ",
	"pous_podos",		"pous",	"",		" ",
	"klehs_kleous",		"cles",	"",		"masc",
	"is_ios",		"is",	"",		"fem ",
	"is_idos",		"is",	"",		"fem ",
	"is_ews",		"is",	"",		"fem ",
	"eus_ews",		"eus",	"",		"masc ",
	"eus_ews",		"ian",	"",		"masc ",
	"eus_ews",		"an",	"",		"masc ",
	"as_ados",		"adian", "",		"fem ",
	"as_antos",		"as",	"",		"masc ",
	"a_hs",			"a",	"",		"fem ",
	"h_hs",			"e",	"",		"fem ",
	"h_hs",			"a",	"",		"fem doric",
	"h_hs",			"ae",	"",		"fem pl",
	"hs_ou",		"",	"",		"masc ",
	"hs_ou",		"ae",	"",		"masc pl",
	"hs_ou",		"as",	"",		"masc doric",
	"hs_eos",		"crates",	"krat",		"masc ",
	"hs_ou",		"es",	"",		"masc ",
	"hs_eos",		"es",	"",		"masc ",
	"os_ou",		"us",	"",		"masc ",
	"os_ou",		"um",	"",		"neut ",
	"os_h_on",		"ian",	"",		"",
	"os_h_on",		"an",	"",		"",
	"os_h_on",		"ans",	"",		"",
	{0},
};


int checkposs(FILE *fout, char *xlit, char *curs, char *fulls, char *stems, char *ends)
{
	(void)fout;
	char tmpxlit[BUFSIZ];
	char tmpstem[MAXWORDSIZE];
	char tmpkeys[MAXWORDSIZE];
	int i = 0;
	int hits = 0;
	int rval = 0;

	Xstrncpy(tmpstem,stems,sizeof(tmpstem));
	stripacc(tmpstem);
	
	for(i=0;Nom_tab[i].stype[0];i++) {
		if( ! strcmp(curs,Nom_tab[i].stype) ) {
			Xstrncpy(tmpxlit,xlit,sizeof(tmpxlit));
			Xstrncat(tmpxlit,Nom_tab[i].suffix,sizeof(tmpxlit));
			if( is_propname(tmpxlit) ) {
				snprintf(tmpkeys,sizeof(tmpkeys),"%s", Nom_tab[i].keys );
				rval = stem_can_work(tmpxlit,fulls, tmpstem, ends, Nom_tab[i].stype, tmpkeys );
/*
				sprintf(stemkeys,"%s %s %s %s",tmpxlit, tmpstem, Nom_tab[i].stype, Nom_tab[i].keys );
				subchar(stemkeys,' ',':');
				set_stem(Gkword,tmpstem);
				set_endstring(Gkword,ends);
				set_gkstring(curstem,tmpstem);
				set_workword(Gkword,fulls);
				rval = checkdict(Gkword,curstem,stemkeys );
*/
				if( ! rval ) {
					snprintf(tmpkeys,sizeof(tmpkeys),"%s suff_acc", Nom_tab[i].keys );
					rval = stem_can_work(tmpxlit, fulls,tmpstem, ends, Nom_tab[i].stype, tmpkeys );
				}
				if( ! rval ) {
					snprintf(tmpkeys,sizeof(tmpkeys),"%s stem_acc", Nom_tab[i].keys );
					rval = stem_can_work(tmpxlit, fulls,tmpstem, ends, Nom_tab[i].stype, tmpkeys );
				}
				if( rval && is_propname(tmpxlit) == 1 ) {
					/*fprintf(fout,":ns:%s %s %s %s\n", tmpxlit,  tmpstem, Nom_tab[i].stype, tmpkeys );*/
					snprintf(tbuf,sizeof(tbuf),"%s :ns:%s %s %s %s\n", fulls, tmpxlit, tmpstem, Nom_tab[i].stype, tmpkeys );
					Xstrncat(possbuf,tbuf,sizeof(possbuf));
/*
					printf("%s %s %s %s\n", tmpxlit,  tmpstem, Nom_tab[i].stype, tmpkeys );
*/
				}
				hits += rval;
			}
		}
	}

/*	if( poss == 1 ) fprintf(fout,"%s", possbuf);*/
/*	
	printf("\t%s\t%s\n", xlit, curs );
*/
	return(hits);
}

int stem_can_work(char *englem, char *fulls, char *stem, char *ends, char *stype, char *keys)
{
	gk_word * Gkword;
	gk_string * curstem;
	char stemkeys[BUFSIZ];
	int rval = 0;
	
	Gkword = CreatGkword(1);
	curstem = CreatGkString(1);
	stemkeys[0] = 0;

	snprintf(stemkeys,sizeof(stemkeys),"%s %s %s %s",englem, stem, stype, keys );
	subchar(stemkeys,' ',':');
	set_stem(Gkword,stem);
	set_endstring(Gkword,ends);
	set_gkstring(curstem,stem);
	set_workword(Gkword,fulls);
	rval = checkdict(Gkword,curstem,stemkeys );

	
	FreeGkword(Gkword);
	FreeGkString(curstem);
	return(rval);
}

typedef struct {
	char raw[20];
	char done[20];
} xlit_tab;


xlit_tab Xlit_list[] = {
	"aue",	"ave",
	"auh",	"ave",
	"eui",	"evi",
	"eue",	"eve",
	"euh",	"eve",
	"eua",	"eva",
	"aui",	"avi",
	"au",	"au",
	"eu",	"eu",
	"h",	"e",
	"k",	"c",
	"c",	"x",
	"q",	"th",
	"u",	"y",
	"f",	"ph",
	"x",	"ch",
	"y",	"ps",
	"w",	"o",
	"ai",	"ae",
	"oi",	"oe",
	"ou",	"u",
	"rr",	"rrh",
	"ei",	"i",
	"gg",	"ng",
	"gx",	"nch",
	"gk",	"nc",
	{0},
	};
	
void reliterate(char *s)
{
	char tmp[BUFSIZ];
	int i = 0;
	int sawone = 0;

	while(*s) {
		for(i=0;Xlit_list[i].raw[0];i++) {
			if( ! Xstrncmp(s,Xlit_list[i].raw,Xstrlen(Xlit_list[i].raw) ) ) {
				Xstrncpy(tmp,s+Xstrlen(Xlit_list[i].raw),sizeof(tmp));
				Xstrncpy(s,Xlit_list[i].done,MAXWORDSIZE);
				Xstrncat(s,tmp,MAXWORDSIZE);
				s += Xstrlen(Xlit_list[i].done);
				sawone = 1;
			}
		}

		if( sawone)
			sawone = 0;
		else
			s++;
	}

}


int AlphStrcmp(char *s1, char *s2)
{
	char * p = s2;
	char tmp[MAXWORDSIZE];

	Xstrncpy(tmp,s1,sizeof(tmp));
	while(*p) {
		if( *p == 'a' && *(tmp+(p-s2)) == 'e' ) {
			*(tmp+(p-s2)) = 'a';
		}
		p++;
	}
	return(strcmp(tmp,s2));
}

char *pnametab[10000];
int pnames = 0;

void init_pnametab(void)
{
	char line[BUFSIZ];

	if( pnames ) return;
	for(pnames=0;pnames<10000;pnames++) {
		if( ! fgets(line,sizeof line,fpnames) ) {
			if( pnames == 1 ) pnames = 0;
			break;
		}
		if(line[strlen(line)-1] == '\n') line[strlen(line)-1] = 0;
		pnametab[pnames] = (char*) malloc((size_t)strlen(line)+1);
		if( !pnametab[pnames] ) break;
		Xstrncpy(pnametab[pnames],line,BUFSIZ);
	}

}

int is_propname(char *s)
{
	char line[BUFSIZ];
	int i = 0;
	
	if( ! pnames ) init_pnametab();
	if( ! pnames ) return(0);

	fseek(fpnames,0L,0);
	for(i=0;i<pnames;i++) {
		if( ! strcmp(s,pnametab[i]) ) return(1);
		if( ! AlphStrcmp(s,pnametab[i]) ) return(2);
	}
	return(0);

	fseek(fpnames,0L,0);
	while( fgets(line,sizeof line,fpnames) ) {
		line[Xstrlen(line)-1] = 0;
		if( ! strcmp(s,line) ) return(1);
		if( ! AlphStrcmp(s,line) ) return(2);
	}
	return(0);
}


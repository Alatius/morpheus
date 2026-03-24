%option noyywrap main

%{
#include <ctype.h>
void dump_adv(void);
void set_lemma(char *p);
void set_orth(int tlen);
void set_orthalt(int tlen);
void dump_adj1(int n);
void dump_form(char *p);
void dump_form2(int n);
void dump_form3(int n);
int sameend(char *s1, char *s2);
int unique_lastc(char *s, int n, int maxc);
void dump_adj(int trimn, char *stype, char *glab, char *nlab);
void dump_nom(int trimn, char *stype, char *glab, char *nlab);
void dump_ent(int trimn, char *etag, char *stype, char *glab, char *nlab);
int is_itype(unsigned char *s, unsigned char *ends);
void trimstem(unsigned char *curs, int trimn);
int is_prevborth(char *s);
%}

%%

^[A-Za-z][a-z\^\_\-]+"ter\t"$	dump_adv();
^[A-Za-z\^\_\-\+]+[#1-9]* 	set_lemma(yytext);

"<orth type=alt>"[A-Za-z][^ <]+[\-\.]"</orth>"	set_orthalt(15);
"<orth>"[A-Za-z][^ <]+[\-\.]"</orth>"	set_orthalt(6);
"<orth>"[A-Za-z][^ <]+[a-z]"</orth>"	set_orth(4);
"<orth>"[A-Za-z][^ <]+[a-z][;:]"</orth>"	set_orth(4);
"<itype>"[^<]+"</itype>\t<pos>adj.</pos>"	dump_adj1(7);
"<itype>"[^<]+"</itype>\t<gen>"[^<]+"</gen>"	dump_form(yytext+yyleng-8);
"<itype>"[^<]+"</itype>"	dump_form2(7);
"<gen>"[^<]+"</gen>"	dump_form2(5);
"<pos>"[^<]+"</pos>"	dump_form3(5);

%%

#include <string.h>
#define MAXORTH 12
unsigned char orthtab[MAXORTH][BUFSIZ];

unsigned char curlemma[BUFSIZ], curstem[BUFSIZ], stemnoacc[BUFSIZ], curitype[BUFSIZ];
unsigned char curpat[BUFSIZ];
int dumped_stem = 0;


void dump_adv()
{
	set_lemma(yytext);
	printf("\n:le:%s\n:wd:%s\tadverb\n", curlemma, yytext );
}

void set_lemma(char *p)
{
	char * t = curlemma;
	int i;

	dumped_stem = 0;
	for(i=0;i<MAXORTH;i++) orthtab[i][0] = 0;

	ECHO;
	strcpy(curlemma,p);
	while(*t) {
		if( !isalpha(*t) && *t!='#' && !isdigit(*t) ) {
			memmove(t,t+1,strlen(t+1)+1);
			continue;
		}
		t++;
	}
	strcpy(stemnoacc,curlemma);
	if( strchr(stemnoacc,'#') ) *(strchr(stemnoacc,'#')) = 0;
	

	strcpy(curstem,yytext);

	if( strchr(curstem,'#') ) *(strchr(curstem,'#')) = 0;
}

void set_orth(int tlen)
{
	unsigned char curpat[BUFSIZ], *s;
	
	ECHO;
	yytext[yyleng-tlen-3] = 0;

	strcpy(curpat,yytext+tlen+2);

	s = curpat; while(*s) s++; s--;
	if(*s == ';' || *s == ':' ) *s = 0;

	if( sameend(curpat,curstem) ) {
		strcpy(orthtab[0],curpat);
	}
}

int sameend(char *s1, char *s2)
{
	/* don't need an alternate if the two are the same */
	if( ! strcmp(s1,s2)) return(0);
	if(*s1) {
		while(*s1) s1++; s1--;
	}


	if(*s2) {
		while(*s2) s2++; s2--;
	}
	return(*s1==*s2);
}

void set_orthalt(int tlen)
{
	unsigned char curpat[BUFSIZ], *s;
	int rval;
	int lastc;

	ECHO;
	yytext[yyleng-8] = 0;
	strcpy(curpat,yytext+tlen);

	if( !strcmp(curpat,"trisyl.") || ! strcmp(curpat,"dissyl.")
	   || !strcmp(curpat,"quadrisyl.") ) return;

	s = strchr(curpat,0);
	while(s>=curpat&&!isalpha(*s)) s--;
	lastc = *s;
	if( unique_lastc(curstem,lastc,strlen(curpat)+2) ) {
		strcpy(orthtab[0],curpat);
		strcat(orthtab[0],strchr(curstem,lastc)+1);
	} else if( (rval=is_prevborth(curpat)) ) {
		char * t1, *t2;
		int i;
		strcpy(orthtab[0],curstem);
/*
 * e.g. acclamatio with orth "adc" --> adclamatio 
 */

		t1 = orthtab[0];
		for(i=0;i<rval;i++) {
			while(*t1&&!isalpha(*t1)) 
				t1++; /* point to first alphabetic char*/
			t1++;
		}
		while(*t1&&!isalpha(*t1)) t1++; /* point to 2nd alphabetic char*/
		t2 = t1;
		t2++;
		while(*t1&&!isalpha(*t2)) t2++; /* point to 2nd alphabetic char*/
		if( *t2 == *t1 ) {
			*t1 = curpat[rval]; /* unassimilate the 'd' */
		} else orthtab[0][0] = 0;
	}
}

int unique_lastc(char * s, int n,int maxc)
{
	int rval = 0;
	int i = 0;

if( maxc > 2 ) s++;
/*
 * don't fail on "maxumus" vs. "maximus"
 */
	while(*s) {
		if(*s++ == n ) rval++;
		if(++i> maxc ) break;
		
	}
	return(rval == 1);
}

void dump_adj(int trimn,char * stype,char *glab,char * nlab)
{
	dump_ent(trimn,"aj", stype,glab, nlab);
}

void dump_nom(int trimn,char * stype,char *glab,char * nlab)
{
	dump_ent(trimn,"no", stype,glab, nlab);
}

void dump_ent(int trimn,char *etag,char * stype,char *glab,char * nlab)
{

	unsigned char * s;
	char tmp[BUFSIZ];
	int i;
	

	dumped_stem = 1;
	if( ! trimn ) trimn = yytext[7];
	

/*
 * if trimn < 0, then you have a stem like "liber" which you do not
 * want to truncate!
 */

	strcpy(tmp,curstem);
	trimstem(curstem,trimn);
/*
	if( trimn > 0 ) {
		s = curstem;
		while(*s) s++; s--;
		while(s>=curstem && *s != trimn ) s--;
		*s = 0;
	}
*/

	printf("\n:le:%s\n", curlemma );
	printf(":%s:%s\t%s%s%s\n",etag, curstem, stype, glab, nlab );
	
	for(i=0;i<MAXORTH;i++) {
		if( ! orthtab[i][0] ) break;
		trimstem(orthtab[i],trimn);
		printf(":%s:%s\t%s%s%s orth\n\n",etag, orthtab[i], stype, glab, nlab );
	}
	putchar('\n');
	strcpy(curstem,tmp);
}

void dump_form(char *p)
{
	char gendlab[BUFSIZ], numlab[BUFSIZ];
	char * t;

	strcpy(curpat,yytext);
	numlab[0] = 0;
	

	while(p>=yytext) {
		if(*p=='>') {
			p++;
			break;
		}
		p--;
	}

	switch(*p) {
		case 'c':
			strcpy(gendlab," masc fem");
			break;
		case 'm':
			strcpy(gendlab," masc");
			break;
		case 'n':
			strcpy(gendlab," neut");
			break;
		case 'f':
			strcpy(gendlab," fem");
			break;
		default:
			gendlab[0] = 0;
			break;
	}

	t = strchr(curpat,'\t') ;
	if( t ) *t = 0;

	strcpy(curitype,curpat+strlen("<itype>"));
	curitype[strlen(curitype)-strlen("</itype>")]  = 0;


if( is_itype("is","is") )	{ dump_nom('i',"is_is",gendlab,numlab); return; }
if( is_itype("i","i^us") )	{ dump_nom('i',"ius_i",gendlab,numlab); return; }
if( is_itype("i","ius") )	{ dump_nom('i',"ius_i",gendlab,numlab); return; }
if( is_itype("i","us") )	{ dump_nom('u',"us_i",gendlab,numlab); return; }
if( is_itype("i","os") )	{ dump_nom('o',"os_i",gendlab,numlab); return; }
if( is_itype("i","um") )	{ dump_nom('u',"us_i",gendlab,numlab); return; }
if( is_itype("um","es") )	{ dump_nom('e',"0_is",gendlab," pl"); return; }
if( is_itype("um","a") )	{ dump_nom('a',"0_is",gendlab," pl"); return; }
if( is_itype("i","on") )	{ dump_nom('o',"os_i",gendlab,numlab); return; }
if( is_itype("i","er") )	{ dump_nom('e',"er_i",gendlab,numlab); return; }
if( is_itype("ei","eus") )	{ dump_nom('e',"eus_ei",gendlab,numlab); return; }
if( is_itype("vi^ri","ir") )	{ dump_nom(-1,"0_i",gendlab,numlab); return; }
if( is_itype("e^i","eus") )	{ dump_nom('e',"eus_ei",gendlab,numlab); return; }

if( is_itype("y^os","ys") )	{ dump_nom(0,"ys_yos",gendlab,numlab); return; }
if( is_itype("o_nis","o") )	{ dump_nom(0,"o_oLnis",gendlab,numlab); return; }
if( is_itype("o_nis","on") )	{ dump_nom(0,"oLn_oLnis",gendlab,numlab); return; }
if( is_itype("o^nis","on") )	{ dump_nom(0,"oLn_onis",gendlab,numlab); return; }
if( is_itype("o_ris","or") )	{ dump_nom(0,"or_oLris",gendlab,numlab); return; }
if( is_itype("o^ris","or") )	{ dump_nom(0,"or_oris",gendlab,numlab); return; }
if( is_itype("a^ris","ar") )	{ dump_nom(-1,"0_is",gendlab,numlab); return; }
if( is_itype("u^ris","ur") )	{ dump_nom(-1,"0_is",gendlab,numlab); return; }
if( is_itype("o^ris","or") )	{ dump_nom(-1,"0_is",gendlab,numlab); return; }
if( is_itype("o^ris","us") )	{ dump_nom('u',"us_oris"," neut",numlab); return; }
if( is_itype("oris","or") )	{ dump_nom(0,"or_oris",gendlab,numlab); return; }
if( is_itype("e_dis","es") )	{ dump_nom('e',"es_eLdis",gendlab,numlab); return; }
if( is_itype("y^dis","ys") )	{ dump_nom('s',"s_dis",gendlab,numlab); return; }
if( is_itype("pe^dis","pes") )	{ dump_nom('p',"peLs_pedis",gendlab,numlab); return; }

if( is_itype("e_s","e") )	{ dump_nom(0,"e_es",gendlab,numlab); return; }
if( is_itype("es","e") )	{ dump_nom(0,"e_es",gendlab,numlab); return; }
/*
if( is_itype("i^dis","is") )	{ dump_nom('s',"s_dis",gendlab,numlab); return; }
*/
if( is_itype("i^dis","is") )	{ dump_nom(0,"is_idis",gendlab,numlab); return; }
if( is_itype("i_cis","ix") )	{ dump_nom(0,"ix_iLcis",gendlab,numlab); return; }
if( is_itype("i^cis","ix") )	{ dump_nom(0,"ix_icis",gendlab,numlab); return; }
if( is_itype("i^cis","ex") )	{ dump_nom('e',"ex_icis",gendlab,numlab); return; }
if( is_itype("e^o_s","eus") )	{ dump_nom('e',"eus_ei",gendlab,numlab); return; }
if( is_itype("e^o_s","is") )	{ dump_nom('i',"is_eos",gendlab,numlab); return; }
if( is_itype("e^os","eus") )	{ dump_nom('e',"eus_ei",gendlab,numlab); return; }
if( is_itype("e^os","is") )	{ dump_nom('i',"is_eos",gendlab,numlab); return; }
if( is_itype("e^os","os") )	{ dump_nom('o',"is_eos",gendlab,numlab); return; }
if( is_itype("a^cis","ax") )	{ dump_nom('x',"x_cis",gendlab,numlab); return; }
if( is_itype("e^ris","us") )	{ dump_nom('u',"us_eris",gendlab,numlab); return; }
if( is_itype("e^ris","is") )	{ dump_nom('i',"is_eris",gendlab,numlab); return; }
if( is_itype("e^ris","er") )	{ dump_nom('e',"er_eris",gendlab,numlab); return; }
if( is_itype("e_ris","er") )	{ dump_nom('e',"eLr_eLis",gendlab,numlab); return; }
if( is_itype("a_ris","ar") )	{ dump_nom(-1,"0_is",gendlab,numlab); return; }
if( is_itype("a_lis","al") )	{ dump_nom(-1,"0_is",gendlab,numlab); return; }
if( is_itype("e_nis","al") )	{ dump_nom(-1,"0_is",gendlab,numlab); return; }
if( is_itype("o^pis","ps") )	{ dump_nom('s',"s_is",gendlab,numlab); return; }
if( is_itype("actis","ax") )	{ dump_nom('x',"x_ctis",gendlab,numlab); return; }
if( is_itype("noctis","ox") )	{ dump_nom('x',"x_ctis",gendlab,numlab); return; }
if( is_itype("o_is","os") )	{ dump_nom('o',"os_ois",gendlab,numlab); return; }

if( is_itype("o_rum","i") )	{ dump_nom('i',"us_i",gendlab," pl"); return; }
if( is_itype("o_rum","a") )	{ dump_nom('a',"us_i",gendlab," pl"); return; }
if( is_itype("a_rum","ae") )	{ dump_nom('a',"a_ae",gendlab," pl"); return; }
if( is_itype("arum","ae") )	{ dump_nom('a',"a_ae",gendlab," pl"); return; }

if( is_itype("is","es") )	{ dump_adj('e',"es_is",gendlab,""); return; }
if( is_itype("u_s","us") )	{ dump_nom(0,"us_us",gendlab,""); return; }
if( is_itype("u_s","u") )	{ dump_nom(0,"us_us"," neut",""); return; }
if( is_itype("u_s","o") )	{ dump_nom('o',"o_us",gendlab,""); return; }
if( is_itype("a_tis","tas") )	{ dump_nom('t',"tas_tatis",gendlab,""); return; }
if( is_itype("u_tis","us") )	{ dump_nom('s',"s_tis",gendlab,""); return; }
if( is_itype("o_tis","os") )	{ dump_nom('s',"s_tis",gendlab,""); return; }
if( is_itype("i^nis","o") )	{ dump_nom('o',"o_inis",gendlab,""); return; }
if( is_itype("i^nis","is") )	{ dump_nom('s',"s_nis",gendlab,""); return; }
if( is_itype("i^nis","men") )	{ dump_nom('m',"men_minis",gendlab,""); return; }
if( is_itype("is","es") )	{ dump_nom('e',"es_is",gendlab,""); return; }
if( is_itype("is","e") )	{ dump_nom('e',"is_is",gendlab,""); return; }
if( is_itype("i^um","es") )	{ dump_nom('e',"is_is",gendlab," pl"); return; }
if( is_itype("i^um","ia") )	{ dump_nom('i',"is_is",gendlab," pl"); return; }
if( is_itype("i^um","a") )	{ dump_nom('a',"is_is",gendlab," pl"); return; }
if( is_itype("ium","es") )	{ dump_nom('e',"is_is",gendlab," pl"); return; }
if( is_itype("ium","ia") )	{ dump_nom('i',"is_is",gendlab," pl"); return; }
if( is_itype("ium","a") )	{ dump_nom('a',"is_is",gendlab," pl"); return; }
if( is_itype("a^tis","ma") )	{ dump_nom('m',"ma_matis",gendlab,""); return; }
if( is_itype("e_tis","es") )	{ dump_nom('e',"es_eLtis",gendlab,""); return; }
if( is_itype("e^tis","es") )	{ dump_nom('s',"s_tis",gendlab,""); return; }
if( is_itype("e_i","es") )	{ dump_nom('e',"es_ei",gendlab,""); return; }
if( is_itype("antis","as") )	{ dump_nom('a',"as_antis",gendlab,""); return; }
if( is_itype("ontis","on") )	{ dump_nom('o',"on_ontis",gendlab,""); return; }
if( is_itype("ntis","on") )	{ dump_nom('o',"on_ontis",gendlab,""); return; }
if( is_itype("i^tis","es") )	{ dump_nom('e',"es_itis",gendlab,""); return; }
if( is_itype("entis","ens") )	{ dump_nom('e',"ens_entis",gendlab,""); return; }
if( is_itype("untis","uns") )	{ dump_nom('e',"uns_untis",gendlab,""); return; }

if( is_itype("ii","us") )	{ dump_nom(0,"ius_i",gendlab,""); return; }
if( is_itype("i^i","us") )	{ dump_nom(0,"ius_i",gendlab,""); return; }
if( is_itype("ii","um") )	{ dump_nom(0,"ius_i",gendlab,""); return; }
if( is_itype("ii","on") )	{ dump_nom(0,"ios_i",gendlab,""); return; }
if( is_itype("i^i","on") )	{ dump_nom(0,"ios_i",gendlab,""); return; }
if( is_itype("i^i","um") )	{ dump_nom(0,"ius_i",gendlab,""); return; }

if( is_itype("ae","a") )	{ dump_nom(0,"a_ae",gendlab,numlab); return; }
if( is_itype("ae","e") )	{ dump_nom('e',"a_ae",gendlab,numlab); return; }
if( is_itype("ae","as") )	{ dump_nom('a',"as_ae",gendlab,numlab); return; }
if( is_itype("ae","es") )	{ dump_nom('e',"es_ae",gendlab,numlab); return; }
if( is_itype("a^dis","as") )	{ dump_nom('a',"as_adis",gendlab,numlab); return; }
if( is_itype("tri","er") )	{ dump_nom('e',"er_ri",gendlab,numlab); return; }
if( is_itype("bris","er") )	{ dump_nom('e',"er_ris",gendlab,numlab); return; }
if( is_itype("pri","er") )	{ dump_nom('e',"er_ri",gendlab,numlab); return; }
if( is_itype("bri","er") )	{ dump_nom('e',"er_ri",gendlab,numlab); return; }
if( is_itype("vi^ri","er") )	{ dump_nom(-1,"0_i",gendlab,numlab); return; }
if( is_itype("be^ri","er") )	{ dump_nom('e',"er_ri",gendlab,numlab); return; }
if( is_itype("e^ri","er") )	{ dump_nom('e',"er_ri",gendlab,numlab); return; }
if( is_itype("u_ris","us") )	{ dump_nom('u',"uLs_uLris",gendlab,numlab); return; }
if( is_itype("tis","s") )	{ dump_nom('s',"s_tis",gendlab,numlab); return; }
if( is_itype("untis","us") )	{ dump_nom('u',"us_untis",gendlab,numlab); return; }
if( is_itype("antis","as") )	{ dump_nom('a',"as_antis",gendlab,numlab); return; }
if( is_itype("dri","er") )	{ dump_nom('e',"er_ri",gendlab,numlab); return; }
if( is_itype("i^inis","men") )	{ dump_nom('m',"men_minis",gendlab,numlab); return; }


ECHO;
}

int is_itype(unsigned char *s,unsigned char *ends)
{
	int n;

	if( strcmp(s,curitype)) return(0);

	if( ! * ends ) return(1);

	return(! (strcmp(ends,stemnoacc+strlen(stemnoacc)-strlen(ends))) );
}

void dump_form2(int n)
{
	strcpy(curitype,yytext+n);
	curitype[strlen(curitype)-(n+1)]  = 0;

	if( is_itype("y^os","ys") )	{ dump_nom(0,"ys_yos"," masc",""); return; }
	if( is_itype("i^nis","is") )	{ dump_nom('s',"s_nis"," masc",""); return; }
if( is_itype("a, um","i^us") )	{ dump_adj('i',"ius_ia_ium","",""); return; }
if( is_itype("a, um","ius") )	{ dump_adj('i',"ius_ia_ium","",""); return; }
if( is_itype("a","i^us") )	{ dump_adj('i',"ius_ia_ium","",""); return; }
if( is_itype("a","ius") )	{ dump_adj('i',"ius_ia_ium","",""); return; }
if( is_itype("a","us") )	{ dump_adj('u',"us_a_um","",""); return; }
if( is_itype("ci^pi^tis","eps") )	{ dump_adj('e',"eps_ipitis"," masc fem neut",""); return; }
	if( is_itype("a, um","us") )	{ dump_adj('u',"us_a_um","",""); return; }
	if( is_itype("a_cis","ax") )	{ dump_adj('a',"ax_adj","",""); return; }
	if( is_itype("i_cis","ix") )	{ dump_adj('i',"ix_adj","",""); return; }
	if( is_itype("o_cis","ox") )	{ dump_adj('o',"ox_adj","",""); return; }
	if( is_itype("o^pis","ps") )	{ dump_adj('s',"s_e","",""); return; }
	if( is_itype("u^ris","ur") )	{ dump_adj(-1,"0_is"," masc",""); return; }
	if( is_itype("e","is") )	{ dump_adj('i',"is_e","",""); return; }
	if( is_itype("is","is") )	{ dump_adj('i',"is_e","",""); return; }
	if( is_itype("tra, trum","ter") )	{ dump_adj('e',"er_ra_rum","",""); return; }
	if( is_itype("chra, chrum","er") )	{ dump_adj('e',"er_ra_rum","",""); return; }

	if( is_itype("a^dis","as") )	{ dump_nom('a',"as_adis"," fem",""); return; }
	if( is_itype("vi^ri","ir") )	{ dump_nom(-1,"0_i"," masc",""); return; }
	if( is_itype("i^nis","men") )	{ dump_nom('m',"men_minis"," neut",""); return; }
	if( is_itype("a_rum","ae") )	{ dump_nom('a',"a_ae"," fem"," pl"); return; }
	if( is_itype("dri","er") )	{ dump_nom('e',"er_ri"," masc",""); return; }
	if( is_itype("i^tis","es") )	{ dump_nom('e',"es_itis"," masc fem",""); return; }
	if( is_itype("a_tis","as") )	{ dump_nom('t',"tas_tatis"," fem",""); return; }
	if( is_itype("e^tis","es") )	{ dump_nom('s',"s_tis"," masc fem",""); return; }
	if( is_itype("e_tis","es") )	{ dump_nom('e',"es_eLtis"," masc",""); return; }
	if( is_itype("antis","as") )	{ dump_nom('a',"as_antis"," masc",""); return; }
	if( is_itype("untis","us") )	{ dump_nom('u',"us_untis"," fem",""); return; }
if( is_itype("o^ris","or") )	{ dump_nom(-1,"0_is"," masc",""); return; }
if( is_itype("o^ris","us") )	{ dump_nom('u',"us_oris"," neut",""); return; }

	if( is_itype("o_rum","i") )	{ dump_nom('i',"us_i"," masc"," pl"); return; }
	if( is_itype("o_rum","a") )	{ dump_nom('a',"us_i"," neut"," pl"); return; }
	if( is_itype("ae","a") )	{ dump_nom('a',"a_ae"," fem",""); return; }
	if( is_itype("m.","us") )	{ dump_nom('u',"us_i"," masc",""); return; }
	if( is_itype("m.","is") )	{ dump_nom('i',"is_is"," masc",""); return; }
	if( is_itype("i","um") )	{ dump_nom('u',"us_i"," neut",""); return; }
	if( is_itype("ae","es") )	{ dump_nom('e',"es_ae"," masc",""); return; }
	if( is_itype("i^dis","is") )	{ dump_nom('s',"s_dis"," fem",""); return; }
	if( is_itype("y^dis","ys") )	{ dump_nom('s',"s_dis"," fem",""); return; }
	if( is_itype("o_tis","os") )	{ dump_nom('s',"s_tis"," masc fem",""); return; }
	if( is_itype("i^nis","o") )	{ dump_nom('o',"o_inis"," masc",""); return; }
	if( is_itype("adj.","us") )	{ dump_nom('u',"us_a_um"," masc",""); return; }
	if( is_itype("m.","os") )	{ dump_nom('o',"os_i"," masc",""); return; }
	if( is_itype("f.","us") )	{ dump_nom('u',"us_i"," fem",""); return; }
	if( is_itype("f.","os") )	{ dump_nom('o',"os_i"," fem",""); return; }
	if( is_itype("f.","ae") )	{ dump_nom('a',"a_ae"," fem"," pl"); return; }
	if( is_itype("f.","e") )	{ dump_nom('e',"e_es"," fem",""); return; }
	if( is_itype("f.","e") )	{ dump_nom('e',"is_is"," neut",""); return; }
	if( is_itype("f.","ies") )	{ dump_nom('e',"es_eLi"," fem",""); return; }
	if( is_itype("f.","tas") )	{ dump_nom('t',"tas_tatis"," fem",""); return; }
	if( is_itype("f.","x") )	{ dump_nom('x',"x_cis"," fem",""); return; }
	if( is_itype("m.","x") )	{ dump_nom('x',"x_cis"," masc",""); return; }
	if( is_itype("f.","io") )	{ dump_nom('o',"o_oLnis"," fem",""); return; }
	if( is_itype("f.","o") )	{ dump_nom('o',"o_us"," fem",""); return; }
	if( is_itype("f.","a") )	{ dump_nom('a',"a_ae"," fem",""); return; }
	if( is_itype("f.","is") )	{ dump_nom('i',"is_is"," fem",""); return; }
	if( is_itype("f.","ae") )	{ dump_nom('a',"a_ae"," fem"," pl"); return; }
	if( is_itype("e_dis","es") )	{ dump_nom('e',"es_eLdis"," masc fem",""); return; }
	if( is_itype("m.","is") )	{ dump_nom('u',"is_is"," masc",""); return; }
	if( is_itype("n.","a") )	{ dump_nom('a',"us_i"," neut"," pl"); return; }
	if( is_itype("n.","men") )	{ dump_nom('m',"men_minis"," neut",""); return; }
	if( is_itype("n.","um") )	{ dump_nom('u',"us_i"," neut",""); return; }
	if( is_itype("n.","on") )	{ dump_nom('o',"os_i"," neut",""); return; }

	ECHO;
}

void dump_adj1(int n)
{
	char * t;

	strcpy(curpat,yytext);
	t = strchr(curpat,'\t');
	if(t) *t = 0;

	strcpy(curitype,curpat+n);
	curitype[strlen(curitype)-(n+1)]  = 0;


if( is_itype("a, um","i^us") )	{ dump_adj('i',"ius_ia_ium","",""); return; }
if( is_itype("a, um","ius") )	{ dump_adj('i',"ius_ia_ium","",""); return; }
if( is_itype("a","i^us") )	{ dump_adj('i',"ius_ia_ium","",""); return; }
if( is_itype("a","ius") )	{ dump_adj('i',"ius_ia_ium","",""); return; }
if( is_itype("a","us") )	{ dump_adj('u',"us_a_um","",""); return; }
if( is_itype("o^ris","or") )	{ dump_adj(-1,"0_adj3","",""); return; }
if( is_itype("i^cis","ex") )	{ dump_adj('e',"ex_icis_adj","",""); return; }
	if( is_itype("a, um","us") )	{ dump_adj('u',"us_a_um","",""); return; }
	if( is_itype("fe^ra, fe^rum","er") )	{ dump_adj('e',"er_era_erum","",""); return; }
	if( is_itype("tris, e","er") )	{ dump_adj('e',"er_ris_re","",""); return; }
	if( is_itype("ci^pi^tis","eps") )	{ dump_adj('e',"eps_ipitis"," masc fem neut",""); return; }
	if( is_itype("pe^dis","pes") )	{ dump_adj('p',"peLs_pedis_adj","",""); return; }
	if( is_itype("te^ra, te^rum","er") )	{ dump_adj('e',"er_era_erum","",""); return; }
	if( is_itype("tra, trum","er") )	{ dump_adj('e',"er_ra_rum","",""); return; }
	if( is_itype("chra, chrum","er") )	{ dump_adj('e',"er_ra_rum","",""); return; }
	if( is_itype("gra, grum","er") )	{ dump_adj('e',"er_ra_rum","",""); return; }
	if( is_itype("noctis","ox") )	{ dump_adj('x',"x_ctis","",""); return; }
	if( is_itype("a_cis","ax") )	{ dump_adj('a',"ax_adj","",""); return; }
	if( is_itype("i_cis","ix") )	{ dump_adj('i',"ix_adj","",""); return; }
	if( is_itype("o_cis","ox") )	{ dump_adj('o',"ox_adj","",""); return; }
	if( is_itype("i^dis","is") )	{ dump_nom('i',"is_idis"," masc fem",""); return; }
	if( is_itype("o^pis","ps") )	{ dump_adj('s',"s_e","",""); return; }
	if( is_itype("is","is") )	{ dump_adj('i',"is_e","",""); return; }
	if( is_itype("e","is") )	{ dump_adj('i',"is_e","",""); return; }
	if( is_itype("entis","ens") )	{ dump_adj('e',"ens_adj","",""); return; }
	if( is_itype("ntis","ens") )	{ dump_adj('e',"ens_adj","",""); return; }
	if( is_itype("e^ra, e^rum","er") )	{ dump_adj('e',"er_era_erum","",""); return; }
	if( is_itype("o_ris","r") )	{ dump_adj(-1,"0_e","",""); return; }

	ECHO;

}
void dump_form3(int n)
{
	if( dumped_stem ) {
		ECHO;
		return;
	}


	if( !strcmp("<pos>adv.</pos>",yytext ) ) {
		printf("\n:le:%s\n", curlemma );
		printf(":wd:%s\tadverb\n\n", curstem );
		if( orthtab[0][0] )
			printf(":wd:%s\tadverb orth\n\n", orthtab[0] );
		return;
	}


	dump_form2(n);
}

void trimstem(unsigned char * curs, int trimn)
{
	int n;
	unsigned char * s;

	if( trimn > 0 ) {
		s = curs;
		while(*s) s++; s--;
		
/*
 * kludge! 7/28/98
 * variation of is and es.
 */
		if(trimn=='e'&&!strcmp((s-1),"is")) {
			*(s-1) = 0;
			return;
		}
		if(trimn=='o'&&!strcmp((s-1),"us")) {
			*(s-1) = 0;
			return;
		}
		while(s>=curs && *s != trimn ) {
			s--;
		}
		*s = 0;
	}
}

int is_prevborth(char *s)
{
	if( ! strncmp(s,"con",3) || ! strncmp(s,"sub",2) ) {
		if( strlen(s) == 4 ) return(2);
		return(0);
	}

	if( ! strncmp(s,"ad",2) || ! strncmp(s,"in",2) || !strncmp(s,"ob",2)) {
		if( !strcmp(s,"inpr") ) return(1);

		if( !strcmp(s,"inpl") ) return(1);

		if( strlen(s) == 3 ) return(1);

		return(0);
	}

	return(0);
}

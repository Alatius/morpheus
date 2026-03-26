#include <gkstring.h>
#include "endfiles.h"
#include "nextsufftab.proto.h"
#include "../greeklib/xstrings.proto.h"
#include "../morphlib/morphkeys.proto.h"
#include "../morphlib/endio.proto.h"
#include "../morphlib/errormess.proto.h"
#include "../morphlib/gkstring.proto.h"
#include "../morphlib/morphstrcmp.proto.h"
#include "../morphlib/nextkey.proto.h"
#include "../greeklib/hasdiaer.proto.h"
#include "../greeklib/hasaccent.proto.h"
#include "../greeklib/hasquant.proto.h"
#include "../greeklib/stripacc.proto.h"
#include "../greeklib/stripdiaer.proto.h"
#include "../greeklib/stripquant.proto.h"
#define MAX_END_TABLE	20000
static char ** endlines;
static int endcount = 0;
static gk_string Gstr;
static gk_string Blnk;
static int xstrcmp(char **, char **);
#define DELIMITER " "

void indexendtables(Stemtype stype, int is_deriv)
{

	int index = 0;
	int i;
	char * curtable, *basen, * dirp;
	char shortname[LONGSTRING];
	char curderivname[LONGSTRING];
	char tmp[LONGSTRING*8];
	char prevtag[LONGSTRING];
	char prevkey[LONGSTRING];
	char curtag[LONGSTRING];
	char savestr[LONGSTRING];
	char markedstr[MAXWORDSIZE];
	FILE * finput, *foutput;
	int maxstring = 0;
	
	if( is_deriv ) 
		dirp = DERIVTABLEDIR;
	else
		dirp = ENDTABLEDIR;
	
	endlines = (char **) calloc(MAX_END_TABLE,sizeof *endlines);
	
	for(;;) {
		if( is_deriv ) {
			gk_string * gstring;
			gk_word * tmpGkword;
			int rconj;
		
			curtable = NextSuffTable(tmp);
			nextkey(tmp,curderivname);
			if( ! curtable ) break;
			curtable = curderivname;
			gstring = CreatGkString(1);
			tmpGkword = CreatGkword(1);
			ScanAsciiKeys(curtable,tmpGkword,gstring,NULL);
			rconj = Is_regconj(gstring);
			
			FreeGkString(gstring);
			FreeGkword(tmpGkword);
			if( ! rconj ) {
				printf("[%s] not a regular conj [%o] [%o]\n", curtable , derivtype_of(gstring), REG_DERIV);
				continue;
			}
		} else 
			curtable=NextEndTable(&index,stype);
		if( ! curtable ) break;
		
/*
		printf("about to compile [%s]\n", curtable );
*/
		snprintf(shortname, sizeof(shortname), "%s%cout%c%s.out", dirp, DIRCHAR, DIRCHAR, curtable );

		if(! (finput=MorphFopen(shortname,"rb"))) {
			continue;
		}
		Gstr = Blnk;	
		get_endheader(finput,&maxstring);

		while(ReadEnding(finput,&Gstr,maxstring)) {
			char * sp;
			sp = gkstring_of(&Gstr);
			Xstrncpy(savestr,sp,sizeof(savestr));
			if( has_diaeresis(sp) || hasaccent(sp) || has_quant(sp) ) {
				Xstrncpy(markedstr,sp,sizeof(markedstr));
			} else
				markedstr[0] = 0;
if( *sp < ' ' || *sp > 126 ) printf("bad line name [%s] sp [%s]\n", shortname , sp );
			stripdiaer(sp);
			stripacc(sp);
			stripquant(sp);
			if( *sp != '*' && *(sp+strlen(sp)-1) == '*' )
				*(sp+strlen(sp)-1) = 0;
if(  ! *sp ) {
	printf("null ending in [%s]\n", curtable );
	continue;
}
			if( is_deriv ) {
				Xstrncpy(tmp,gkstring_of(&Gstr),sizeof(tmp));
				Xstrncat(tmp,"\t",sizeof(tmp));
				if( strcmp(gkstring_of(&Gstr),savestr) ) {
					Xstrncat(tmp,savestr,sizeof(tmp));
				}
				SprintGkFlags(&Gstr,tmp,sizeof(tmp),":",0);
			} else
				snprintf(tmp, sizeof(tmp), "%s\t%s", gkstring_of(&Gstr) , NameOfStemtype(stemtype_of(&Gstr) ) );

/*
			if( is_deriv && derivtype_of(&Gstr) ) {
				strcat(tmp,":");
				strcat(tmp,NameOfDerivtype(derivtype_of(&Gstr)));
			}
*/
			if( endcount >= MAX_END_TABLE ) {
				fprintf(stderr,"more than %d endings in table! bye!\n", MAX_END_TABLE );
				break;
			}
			*(endlines+endcount) = (char *)calloc(strlen(tmp)+1,sizeof ** endlines );
			if( ! *(endlines+endcount) ) {
				fprintf(stderr,"ran out of memory at %d endings!\n", endcount );
				return;
			}
			Xstrncpy(*(endlines+endcount),tmp,strlen(tmp)+1);
			endcount++;
		}
/*
printf("deriv [%o] name [%s]\n", derivtype_of(&Gstr), 
NameOfDerivtype(derivtype_of(&Gstr)) );
*/
		if( endcount >= MAX_END_TABLE ) break;

		fclose(finput);
	}

	qsort(endlines,endcount,sizeof * endlines, (int (*)(const void *, const void *))xstrcmp );
	
printf("stype [%o]\n", stype );
	if( is_deriv ) 
		basen = "derivind";
	else if( stype & PPARTMASK ) 
		basen = "vbendind";
	else
		basen = "nendind";
		
	snprintf(shortname, sizeof(shortname), "%s%cindices%c%s", dirp, DIRCHAR, DIRCHAR , basen );

printf("output file:%s\n", shortname );
	if(! (foutput=MorphFopen(shortname,"w"))) {
		ErrorMess("Could not open nendind!");
		return;
	}
	
	prevtag[0] = 0;
	for(i=0;i<endcount;i++) {

		nextkey(*(endlines+i),curtag);
		/*
		 * if a new keys
		 */

		if( morphstrcmp(curtag,prevtag) ) {
			if( prevtag[0] ) fprintf(foutput,"\n");
			fprintf(foutput,"%s%s%s", curtag, DELIMITER, *(endlines+i) );
		} else if ( strcmp(prevkey,*(endlines+i) ) )
			/*
			 * don't include lines such as "uiais perf_act perf_act"
			 * where the same key is repeated
			 */
			fprintf(foutput,"%s%s", DELIMITER, *(endlines+i) );
		Xstrncpy(prevtag,curtag,sizeof(prevtag));
		Xstrncpy(prevkey,*(endlines+i),sizeof(prevkey));
	}

	fprintf(foutput,"\n");
	fclose(foutput);
/*	index_list(shortname,NULL);*/
	for(i=0;i<endcount;i++) free(*(endlines+i));
	free((char *)endlines);

}

static int xstrcmp(char **p1, char **p2)
{
	int rval;

	rval = morphstrcmp(*p1,*p2);
	if( ! rval )
		rval = strcmp(*p1,*p2);

	return(rval);
}

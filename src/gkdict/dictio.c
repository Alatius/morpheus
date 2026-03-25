#include <gkstring.h>
#include <gkdict.h>
#include <endtags.h>

/*int dictstrcmp(), dictstrncmp(), morphstrcmp(), morphstrncmp();*/

#include "../morphlib/morphstrcmp.proto.h"


endtags * VbTags = NULL;
endtags * NomTags = NULL;
static int num_of_ntags = 0;
static int num_of_vtags = 0;
char * vbindex = VBINDEX;
char * nomindex = NOMINDEX;
#include "retrentry.proto.h"
 int Use_hqdict = 0;

#include "dictio.proto.h"
#include "xderivio.proto.h"
#include "../morphlib/errormess.proto.h"
#include "../morphlib/nextkey.proto.h"
#include "../morphlib/preverb.proto.h"
#include "../morphlib/retrentry.proto.h"
#include "../morphlib/trimwhite.proto.h"
#include <endindex.h>
#include "../gkends/endindex.proto.h"
#include "../greeklib/Fclose.proto.h"
#include "../greeklib/isblank.proto.h"
#include "../greeklib/stripacc.proto.h"
#include "../greeklib/stripdiaer.proto.h"
#include "../greeklib/stripmeta.proto.h"
#include "../greeklib/stripquant.proto.h"
#include "../greeklib/xstrings.proto.h"


endtags *
init_dict(char *fname, int *ntags)
{
	if( Use_hqdict ) {
		fname = STEMLIST;
		vbindex = STEMLIST;
		nomindex = STEMLIST;
	}
	return(init_preind(fname,ntags));
}


/*
 * check to see if the current string is an irregular verb.
 */
int chckirrverb(char *irregstr, char *lemmas)
{
	char workstem[MAXWORDSIZE];
	int rval;

	workstem[0] = '1';
	Xstrncpy(workstem+1,irregstr,MAXWORDSIZE);
	stripmetachars(workstem);
	stripacc(workstem);
	rval = chckvstem(workstem,lemmas);
	return(rval);
}


/*
 * check to see if string "indeclstring" is in the indeclinable list. if so, 
 * store any flags associated with it in the ascii string "lemmas".
 * 
 * return 0 if "indeclstring" is not in the indeclinable list.
 */
int chckindecl(char *indeclstr, char *lemmas)
{
	long startoff;
	int rval = 0;
	char tmpindecl[MAXWORDSIZE];
	
	*lemmas = 0;
	if( ! NomTags ) {
		NomTags = init_dict(nomindex,&num_of_ntags);
	}
	tmpindecl[0] = '2';
	Xstrncpy(tmpindecl+1,indeclstr,MAXWORDSIZE);
	stripquant(tmpindecl);
	stripdiaer(tmpindecl);
	stripacc(tmpindecl);

/*
	printf("will look for [%s] in indecl\n", tmpindecl );
*/
	startoff = ChckPreIndex(NomTags,tmpindecl,num_of_ntags,NO,morphstrcmp);
/*
printf("startoff [%ld]\n", startoff );
*/
	if( startoff < 0 ) return(0);
	if( ChckFullIndex(tmpindecl,lemmas,nomindex,startoff,morphstrncmp) ) {
		return(1);
	}

	return(rval);
}

/*
 * check to see if string "derivstr" is in the deriv list. if so, 
 * store any flags associated with it in the ascii string "derivkeys".
 * 
 * return 0 if "indeclstring" is not in the indeclinable list.
 */
int chckderiv(char *derivstr, char *derivkeys)
{
	int rval = 0;
	char tmpderivstr[MAXWORDSIZE];

/*
	*derivkeys = 0;
*/
	tmpderivstr[0] = '3';
	Xstrncpy(tmpderivstr+1,derivstr,MAXWORDSIZE);
	stripquant(tmpderivstr);
	stripdiaer(tmpderivstr);
	rval = chckvstem(tmpderivstr,derivkeys);
	return(rval);
}


int chckstem(char *stemstr, char *stemkeys, int is_nom)
{
	long startoff;
	int rval = 0;
	int rval2 = 0;
	int curntags = 0;
	char tmpkeys[LONGSTRING];
	char * indfile;
	endtags * CurTags = NULL;
	
	stripquant(stemstr);
	stripdiaer(stemstr);
	*stemkeys = tmpkeys[0] = 0;
	if( is_nom )  {
/*
		rval = chcknstem(stemstr,stemkeys);
		rval2 = checkforderiv(stemstr,tmpkeys);
		if( rval2 ) {
			if( rval ) Xstrncat(stemkeys," ",LONGSTRING);
			Xstrncat(stemkeys,tmpkeys,LONGSTRING);
		}
	
		return(rval+rval2);
*/
		indfile = nomindex;
	} else {
		rval = chckvstem(stemstr,stemkeys);
		rval2 = checkforderiv(stemstr,tmpkeys);
		if( rval2 ) {
			if( rval ) Xstrncat(stemkeys," ",LONGSTRING);
			Xstrncat(stemkeys,tmpkeys,LONGSTRING);
		}
	
		return(rval+rval2);
	}
	if( ! *stemstr ) return(0);
	
	
	if( ! VbTags ) {
		VbTags = init_dict(vbindex,&num_of_vtags);
	}
	if( ! NomTags ) {
		NomTags = init_dict(nomindex,&num_of_ntags);
	}
	if( is_nom ) {
		CurTags = NomTags;
		curntags = num_of_ntags;
	} else {
		CurTags = VbTags;
		curntags = num_of_vtags;
	}
	
	startoff = ChckPreIndex(CurTags,stemstr,curntags,is_nom? NO : YES,morphstrcmp);

	if( startoff >= 0 ) 
		rval = ChckFullIndex(stemstr,stemkeys,indfile,startoff,morphstrncmp);

/*
	if( ! rval ) {
		rval = checkforderiv(stemstr,stemkeys);
	}
*/
/*
	if( ! is_nom ) 
		rval2 = checkforderiv(stemstr,tmpkeys);
	if( rval2 ) {
		if( rval ) Xstrncat(stemkeys," ",LONGSTRING);
		Xstrncat(stemkeys,tmpkeys,LONGSTRING);
	}

	return(rval+rval2);
*/
	return(rval);
}

endtags * LemmTags = NULL;
static int num_of_ltags = 0;


/*
 * given a lemma, find out where in the dictionary it shows up
 *
 * note that this routine should be changed so that it returns the actual
 * entry for that particular lemma. returning an offset presupposes that
 * the dictionary represents a single, big file, and this will not be the 
 * case (I don't think) when we have a 10 mbyte dictionary.
 *
 * this file returns a FILE pointer that is open to the place in
 * a dictionary file in which the lemma entry begins.
 *
 * the offset at which the lemma entry begins in that file is
 * stored (redundantly) in *lemmoff
 */
 
FILE * 
 getlemmstart(char *lemma, char *lemmfile, long *lemmoff)
{
	char curtarget[LONGSTRING];
	char line[LONGSTRING];
	long curoff;
	FILE * f = NULL;
	long startoff;
	int comp = 0;
	char shorttag[MAXWORDSIZE];
	
 
 	if( ! LemmTags ) {
		LemmTags = init_preind(WORDLIST,&num_of_ltags);
	}

	Xstrncpy(shorttag,lemma,MAXWORDSIZE);
	stripquant(shorttag);
	shorttag[6] = 0;

	startoff = ChckPreIndex(LemmTags,shorttag,num_of_ltags,NO,morphstrcmp);

	if( (f=MorphFopen(WORDLIST,"r")) == NULL ) {
		fprintf(stderr,"getlemmstart: could not find %s\n", line );
		return(NULL);
	}
	fseek(f,startoff,0);

	Xstrncpy(shorttag,lemma,MAXWORDSIZE);
	stripquant(shorttag);

	snprintf(curtarget,sizeof(curtarget),":le:%s", shorttag );

	while(1) {
		char curlemm[MAXWORDSIZE];

		curoff = ftell(f);
		if( ! fgets(line,(int)sizeof  line , f)) {
			*lemmoff = -1;
			break;
		}
		if( Xstrncmp(line,LEMMTAG,4) )
			continue;
		trimwhite(line);
		nextkey(line,curlemm);

		comp = morphstrcmp(curtarget,curlemm);
/*
printf("comp [%d] curtarget [%s] curlemm [%s]\n", comp , curtarget , curlemm);
*/
		if(! comp ) {
/*
char errbuf[LONGSTRING];
sprintf(errbuf,"curoff %ld targ [%s] lem [%s]\n", curoff , curtarget, curlemm );
ErrorMess(errbuf);
*/
			*lemmoff = curoff;
			break;
		}
		if( comp < 0 ) {
			*lemmoff = -1;
			break;
		}
	}
	if( *lemmoff < 0 ) {
/*
		char errbuf[LONGSTRING];
		sprintf(errbuf,"error: no such lemma as [%s]\n", shorttag );
		ErrorMess(errbuf);
*/
		*lemmfile = 0;
		xFclose(f);
		f = NULL;
		return(NULL);
	}
	
	Xstrncpy(lemmfile,WORDLIST,MAXWORDSIZE); 
	fseek(f,*lemmoff,0);
	return(f);

}

int lemma_exists(char *lemma)
{
	FILE * flemm = NULL;
	char lemmfile[LONGSTRING];
	long lemmoff = 0;
	
	if( (flemm = getlemmstart(lemma,lemmfile,&lemmoff)) == NULL )
		return(0);
	xFclose(flemm);
	flemm = NULL;
	return(1);
}

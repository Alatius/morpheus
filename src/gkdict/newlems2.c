#include <gkstring.h>
#include "../greeklib/checkaccent.proto.h"
#include "../greeklib/nsylls.proto.h"
#include "../greeklib/stripacc.proto.h"
#include "../greeklib/stripmeta.proto.h"
#include "../greeklib/standword.proto.h"
#include "../greeklib/xstrings.proto.h"

int do_simpnom(char *, char*);
int dump_nom(char *stem,int trunc, char * tags);
int probe_fem(char *);
int do_regnom(char*stem,char*key1,char*key2,char*key3);
int dump_entry(char *stem,int trunc, char * tags,char * etag,FILE *);
void dump_acc_nom(char *stem, int trunc, char *tag);
int is_verb(char *);
int is_adverb(char *, char *);
int is_teos(char *);
FILE * fverbs;

int main(void)
{
	char line[BUFSIZ];
	char savel[BUFSIZ];
	char curw[BUFSIZ];
	char firstkey[BUFSIZ];
	char seckey[BUFSIZ];
	char thrdkey[BUFSIZ];
	char *s;

	if( ! (fverbs=fopen("/tmp/lsj.vbs","w"))) {
		fprintf(stderr,"could not open /tmp/lsj.vbs!\n");
		exit(-1);
	}

	while(fgets(line, sizeof line, stdin)) {
		line[strcspn(line, "\n")] = '\0';
		if( ! line[0]) continue;
		if( ! isdigit(line[0])) continue;
		Xstrncpy(savel,line,sizeof(savel));
		firstkey[0] = seckey[0] = thrdkey[0] = 0;
		/* skip tokens 1 and 3, but keep #2 */
		s = strtok(line," \t");
		s = strtok((char * )NULL," \t");
		Xstrncpy(curw,s,sizeof(curw));

		if( is_verb(curw) ) continue;
		s=strtok((char * )NULL," \t");
		s=strtok((char * )NULL," \t");
		if(s) Xstrncpy(firstkey,s,sizeof(firstkey));
		s=strtok((char * )NULL," \t");
		if(s) Xstrncpy(seckey,s,sizeof(seckey));
		s=strtok((char * )NULL," \t");
		if(s) Xstrncpy(thrdkey,s,sizeof(thrdkey));
		if( ! strncmp(firstkey,"<gen",4)) {
		/*	printf("%s %s\n", curw , firstkey );*/
			if( ! do_simpnom(curw,firstkey) ) {
				printf("0:%s\n", savel );
			}
			continue;
		}

		if( ! strncmp(firstkey,"<itype>",7)) {
			if( ! do_regnom(curw,firstkey,seckey,thrdkey) ) {
				printf("0:%s\n", savel );
			}
			continue;
		}
		if( is_adverb(curw,firstkey) ) continue;
		if( is_teos(curw) ) continue;
		printf("0:%s\n", savel );
	}
	fclose(fverbs);
	return 0;
}

int do_simpnom(char* stem,char* gend)
{

	if( !strcmp(stem+strlen(stem)-4,"i/+a") ) {
		dump_nom(stem,3,"h_hs fem");
		return(1);
	}

	if( (!strcmp((stem+strlen(stem)-1),"a") ||
	     !strcmp((stem+strlen(stem)-1),"h"))
		&& !strcmp(gend,"<gen>h(</gen>")) {
		probe_fem(stem);
		return(1);
	}

	if( (!strcmp(stem+strlen(stem)-2,"h/") ||
	     !strcmp(stem+strlen(stem)-2,"a/") ) 
		&& !strcmp(gend,"<gen>h(</gen>")) {
		dump_nom(stem,2,"h_hs fem suff_acc");
		return(1);
	}
	
	if( !strcmp(stem+strlen(stem)-1,"c") 
		&& !strcmp(gend,"<gen>h(</gen>")) {
		dump_nom(stem,1,"c_kos fem");
		return(1);
	}
	
	if( !strcmp(stem+strlen(stem)-2,"ma") 
		&& !strcmp(gend,"<gen>to/</gen>")) {
		dump_nom(stem,2,"ma_matos neut");
		return(1);
	}
	
	if( !strcmp(stem+strlen(stem)-2,"a/") 
		&& !strcmp(gend,"<gen>ta/</gen>")) {
		dump_nom(stem,2,"os_ou neut pl");
		return(1);
	}
	
	if( !strcmp(stem+strlen(stem)-4,"mata") 
		&& !strcmp(gend,"<gen>ta/</gen>")) {
		dump_nom(stem,4,"ma_matos neut pl");
		return(1);
	}
	
	if( !strcmp(stem+strlen(stem)-5,"i/des") 
		&& !strcmp(gend,"<gen>ai(</gen>")) {
		dump_nom(stem,5,"is_idos fem pl suff_acc");
		return(1);
	}
	
	if( !strcmp(stem+strlen(stem)-3,"a/s") 
		&& !strcmp(gend,"<gen>h(</gen>")) {
		dump_nom(stem,2,"s_dos fem stem_acc");
		return(1);
	}
	
	if( !strcmp(stem+strlen(stem)-5,"a/des") 
		&& !strcmp(gend,"<gen>ai(</gen>")) {
		dump_nom(stem,4,"s_dos fem pl stem_acc");
		return(1);
	}
	
	if( !strcmp(stem+strlen(stem)-3,"sis") 
		|| !strcmp(stem+strlen(stem)-3,"cis")
		|| !strcmp(stem+strlen(stem)-3,"yis") ) {
		dump_nom(stem,2,"is_ews fem");
		return(1);
	}
	
	if( !strcmp(stem+strlen(stem)-4,"pais") 
		&& !strcmp(gend,"<gen>o(</gen>")) {
		dump_nom(stem,4,"pais_paidos masc");
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-4,"qric") 
		&& !strcmp(gend,"<gen>o(</gen>")) {
		dump_nom(stem,4,"qric_trixos masc");
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-4,"pous") 
		&& !strcmp(gend,"<gen>o(</gen>")) {
		dump_nom(stem,4,"pous_podos masc");
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-5,"podes") 
		&& !strcmp(gend,"<gen>oi(</gen>")) {
		dump_nom(stem,5,"pous_podos masc pl");
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-3,"ai/") 
		&& !strcmp(gend,"<gen>ai(</gen>")) {
		dump_nom(stem,3,"h_hs fem pl suff_acc");
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-3,"h/s") 
		&& !strcmp(gend,"<gen>o(</gen>")) {
		dump_nom(stem,3,"hs_ou masc suff_acc");
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-2,"w/") 
		&& !strcmp(gend,"<gen>h(</gen>")) {
		dump_nom(stem,2,"w_oos fem suff_acc");
		return(1);
	}

	if(!strcmp(stem+strlen(stem)-3,"o/s") ||
		!strcmp(stem+strlen(stem)-3,"o/n"))  {
		char curg[BUFSIZ], tmptags[BUFSIZ];

		if(!strcmp(gend,"<gen>h(</gen>")) Xstrncpy(curg,"fem",sizeof(curg));
		if(!strcmp(gend,"<gen>to/</gen>")) Xstrncpy(curg,"neut",sizeof(curg));
		else Xstrncpy(curg,"masc",sizeof(curg));

		snprintf(tmptags,sizeof(tmptags),"os_ou suff_acc %s", curg );
		dump_nom(stem,3,tmptags);
		return(1);
	}

	if( (!strcmp(stem+strlen(stem)-2,"oi") ||
		!strcmp(stem+strlen(stem)-3,"oi/"))
		&& !strcmp(gend,"<gen>oi(</gen>")) {
		int syllno, curacc;
		char tmptags[BUFSIZ];

		Xstrncpy(tmptags,"os_ou masc pl",sizeof(tmptags));
		checkaccent(stem,&syllno,&curacc);
		if( syllno == ULTIMA ) {
			Xstrncat(tmptags," suff_acc",sizeof(tmptags));
		} else if( nsylls(stem)>2 && syllno == PENULT ) {
			Xstrncat(tmptags, " stem_acc",sizeof(tmptags));
		}
		if( *(stem+strlen(stem)-1) == '/')
			dump_nom(stem,3,tmptags);
		else
			dump_nom(stem,2,tmptags);
		return(1);
	}

	if( (!strcmp(stem+strlen(stem)-2,"ai") ||
		!strcmp(stem+strlen(stem)-3,"ai/"))
		&& !strcmp(gend,"<gen>oi(</gen>")) {
		int syllno, curacc;
		char tmptags[BUFSIZ];

		Xstrncpy(tmptags,"hs_ou masc pl",sizeof(tmptags));
		checkaccent(stem,&syllno,&curacc);
		if( syllno == ULTIMA ) {
			Xstrncat(tmptags," suff_acc",sizeof(tmptags));
		} else if( nsylls(stem)>2 && syllno == PENULT ) {
			Xstrncat(tmptags, " stem_acc",sizeof(tmptags));
		}
		if( *(stem+strlen(stem)-1) == '/')
			dump_nom(stem,3,tmptags);
		else
			dump_nom(stem,2,tmptags);
		return(1);
	}

	if( (!strcmp(stem+strlen(stem)-2,"hs")  ||
	     !strcmp(stem+strlen(stem)-2,"as"))
		&& !strcmp(gend,"<gen>o(</gen>")) {
		int syllno, curacc;
		char curg[BUFSIZ], tmptags[BUFSIZ];

		Xstrncpy(curg,"masc",sizeof(curg));

		snprintf(tmptags,sizeof(tmptags),"hs_ou %s", curg );
		checkaccent(stem,&syllno,&curacc);
		if( nsylls(stem)>2 && syllno == PENULT ) {
			Xstrncat(tmptags, " stem_acc",sizeof(tmptags));
		}
		dump_nom(stem,2,tmptags);
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-1,"a")
		&& !strcmp(gend,"<gen>ta/</gen>") ) {
		int syllno, curacc;
		char tmptags[BUFSIZ];

		Xstrncpy(tmptags,"os_ou neut pl",sizeof(tmptags));
		checkaccent(stem,&syllno,&curacc);
		if( nsylls(stem)>2 && syllno == PENULT ) {
			Xstrncat(tmptags, " stem_acc",sizeof(tmptags));
		}
		dump_nom(stem,1,tmptags);
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-2,"os")
		&& !strcmp(gend,"<gen>to/</gen>") ) {
		int syllno, curacc;
		char tmptags[BUFSIZ];

		Xstrncpy(tmptags,"hs_eos neut",sizeof(tmptags));
		checkaccent(stem,&syllno,&curacc);
		if( nsylls(stem)>2 && syllno == PENULT ) {
			Xstrncat(tmptags, " stem_acc",sizeof(tmptags));
		}
		dump_nom(stem,2,tmptags);
		return(1);
	}

	if( (!strcmp(stem+strlen(stem)-2,"on")  ||
	     !strcmp(stem+strlen(stem)-2,"o/n"))
		&& !strcmp(gend,"<gen>to/</gen>") ) {
		int syllno, curacc;
		char tmptags[BUFSIZ];

		Xstrncpy(tmptags,"os_ou neut",sizeof(tmptags));
		checkaccent(stem,&syllno,&curacc);
		if( nsylls(stem)>2 && syllno == PENULT ) {
			Xstrncat(tmptags, " stem_acc",sizeof(tmptags));
		}
		dump_nom(stem,2,tmptags);
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-2,"os")
		&& (!strcmp(gend,"<gen>h(</gen>") ||
			!strcmp(gend,"<gen>o(</gen>"))) {
		int syllno, curacc;
		char curg[BUFSIZ], tmptags[BUFSIZ];

		if(!strcmp(gend,"<gen>h(</gen>")) Xstrncpy(curg,"fem",sizeof(curg));
		else Xstrncpy(curg,"masc",sizeof(curg));

		snprintf(tmptags,sizeof(tmptags),"os_ou %s", curg );
		checkaccent(stem,&syllno,&curacc);
		if( nsylls(stem)>2 && syllno == PENULT ) {
			Xstrncat(tmptags, " stem_acc",sizeof(tmptags));
		}
		dump_nom(stem,2,tmptags);
		return(1);
	}

	return(0);
}

void circ2long(char*s)
{
	while(*s) {
	 if( !strncmp("oi=",s,3) || !strncmp("ai=",s,3) || !strncmp("ei=",s,3)
	  || !strncmp("ou=",s,3) || !strncmp("au=",s,3) || !strncmp("eu=",s,3)) {
			s+= 3; 
			continue;
		}
		if((*s=='a'||*s=='i'||*s=='u') && *(s+1) == CIRCUMFLEX)
			*(s+1) = HARDLONG;
		s++;
	}
}

int dump_adj(char *stem,int trunc, char * tags)
{

	return(dump_entry(stem,trunc,tags,":aj:",stdout));
}

int dump_nom(char *stem,int trunc, char * tags)
{

	return(dump_entry(stem,trunc,tags,":no:",stdout));
}

int dump_entry(char *stem,int trunc, char * tags,char * etag, FILE * fout)
{
	char shorts[BUFSIZ];
	Xstrncpy(shorts,stem,sizeof(shorts));
	stripmetachars(stem);
	shorts[strlen(shorts)-trunc] = 0;
	if(shorts[strlen(shorts)-1] == '-')
		shorts[strlen(shorts)-1] = 0;
	/* first -- if you have a circumflex on i,u,a, turn it into a
  	   long mark!
  	*/
	circ2long(shorts);
	stripacc(shorts);
	fprintf(fout,":le:%s\n%s%s %s\n\n", stem,etag,shorts, tags);
	return(1);
}



char * getaccp(char *, int);
int probe_fem(char * stem)
{
	int syllno;
	int curacc;

	/* is it a short alpha? */
		checkaccent(stem,&syllno,&curacc);
		if(syllno == ANTEPENULT)
			dump_nom(stem,1,"a_hs fem");
/*
		else if( syllno == PENULT && curacc == CIRCUMFLEX )
			dump_nom(stem,1,"a_hs fem");
*/
	/* guess not... */
		else dump_nom(stem,1,"h_hs fem");
		return(1);
}

int do_regnom(char*stem,char*key1,char*key2,char * key3)
{

	if( !strcmp("<itype>essa</itype>",key1) && 
	    (!strcmp(stem+strlen(stem)-3,"eis")) ) {
		dump_adj(stem,3,"eis_essa");	
		return(1);
	}

	if( !strcmp("qos</itype>",key1+strlen(key1)-11) && 
	    (!strcmp(stem+strlen(stem)-1,"s")) ) {
		char skeys[BUFSIZ];

		if( !strcmp(key2,"<gen>o(</gen>") && !strcmp(key3,"<gen>h(</gen>"))
			Xstrncpy(skeys,"s_qos masc fem",sizeof(skeys));
		else if( !strcmp(key2,"<gen>o(</gen>") )
			Xstrncpy(skeys,"s_qos masc",sizeof(skeys));
		else if( !strcmp(key2,"<gen>h(</gen>") )
			Xstrncpy(skeys,"s_qos fem",sizeof(skeys));
		else
			Xstrncpy(skeys,"s_qos fem",sizeof(skeys));
		dump_nom(stem,1,skeys);
		return(1);
	}

	if( !strcmp("ggos</itype>",key1+strlen(key1)-12) &&
	    (!strcmp(stem+strlen(stem)-2,"gc")) ) {
		char skeys[BUFSIZ];
		int syllno = 0;
		int curacc = 0;

		if( !strcmp(key2,"<gen>o(</gen>") && !strcmp(key3,"<gen>h(</gen>"))
			Xstrncpy(skeys,"gc_ggos masc fem",sizeof(skeys));
		else if( !strcmp(key2,"<gen>o(</gen>") )
			Xstrncpy(skeys,"gc_ggos masc",sizeof(skeys));
		else if( !strcmp(key2,"<gen>h(</gen>") )
			Xstrncpy(skeys,"gc_ggos fem",sizeof(skeys));
		else
			Xstrncpy(skeys,"gc_ggos fem",sizeof(skeys));

		if(nsylls(stem) > 2 ) {
			if(syllno == PENULT ) Xstrncat(skeys," ant_acc",sizeof(skeys));
			checkaccent(stem,&syllno,&curacc);
		}
		dump_nom(stem,2,skeys);
		return(1);
	}

	if( !strcmp("aktos</itype>",key1+strlen(key1)-13) &&
	    (!strcmp(stem+strlen(stem)-2,"ac")|| !strcmp(stem+strlen(stem)-2,"la")) ) {
		char skeys[BUFSIZ];
		int syllno = 0;
		int curacc = 0;

		if( !strcmp(key2,"<gen>o(</gen>") && !strcmp(key3,"<gen>h(</gen>"))
			Xstrncpy(skeys,"c_ktos masc fem",sizeof(skeys));
		else if( !strcmp(key2,"<gen>o(</gen>") )
			Xstrncpy(skeys,"c_ktos masc",sizeof(skeys));
		else if( !strcmp(key2,"<gen>h(</gen>") )
			Xstrncpy(skeys,"c_ktos fem",sizeof(skeys));
		else
			Xstrncpy(skeys,"c_ktos fem",sizeof(skeys));

		if(nsylls(stem) > 2 ) {
			if(syllno == PENULT ) Xstrncat(skeys," ant_acc",sizeof(skeys));
			checkaccent(stem,&syllno,&curacc);
		}
		dump_nom(stem,(strcmp(stem+strlen(stem)-2,"ac")? 1 : 2),skeys);
		return(1);
	}

	if( !strcmp("gos</itype>",key1+strlen(key1)-11) &&
	    (!strcmp(stem+strlen(stem)-1,"c")) ) {
		char skeys[BUFSIZ];
		int syllno = 0;
		int curacc = 0;

		if( !strcmp(key2,"<gen>o(</gen>") && !strcmp(key3,"<gen>h(</gen>"))
			Xstrncpy(skeys,"c_gos masc fem",sizeof(skeys));
		else if( !strcmp(key2,"<gen>o(</gen>") )
			Xstrncpy(skeys,"c_gos masc",sizeof(skeys));
		else if( !strcmp(key2,"<gen>h(</gen>") )
			Xstrncpy(skeys,"c_gos fem",sizeof(skeys));
		else
			Xstrncpy(skeys,"c_gos fem",sizeof(skeys));

		if(nsylls(stem) > 2 ) {
			if(syllno == PENULT ) Xstrncat(skeys," ant_acc",sizeof(skeys));
			checkaccent(stem,&syllno,&curacc);
		}
		dump_nom(stem,1,skeys);
		return(1);
	}

	if( !strcmp("<itype>a^nos</itype>",key1) && 
	    (!strcmp(stem+strlen(stem)-3,"a_s")) ) {
		dump_adj(stem,3,"as_aina_an");	
		return(1);
	}

	if( !strcmp("<itype>idos</itype>",key1) && 
	    (!strcmp(stem+strlen(stem)-2,"is")) ) {
		dump_acc_nom(stem,2,"is_idos fem");	
/*
		if( *(stem+strlen(stem)-4) == '=' ) 
			dump_nom(stem,2,"is_idos fem stem_acc");	
		else
			dump_nom(stem,2,"is_idos fem");	
*/
		return(1);
	}

	if( (!strcmp("<itype>a_kos</itype>",key1) || !strcmp("<itype>a^kos</itype>",key1))
	    && (!strcmp(stem+strlen(stem)-2,"ac")) ) {
		stem[strlen(stem)-1] = key1[8];
		Xstrncat(stem,"c",BUFSIZ);
		if( !strcmp(key2,"<gen>h(</gen>") )
			dump_nom(stem,1,"c_kos fem");
		else
			dump_nom(stem,1,"c_kos masc");	
		return(1);
	}

	if( !strcmp("<itype>akos</itype>",key1) 
	    && (!strcmp(stem+strlen(stem)-2,"ac")) ) {
		if( !strcmp(key2,"<gen>h(</gen>") )
			dump_nom(stem,1,"c_kos fem");	
		else
			dump_nom(stem,1,"c_kos masc");	
		return(1);
	}

	if( !strcmp("<itype>eos</itype>",key1) && 
	    !strcmp("<gen>to/</gen>",key2) &&
	    (!strcmp(stem+strlen(stem)-2,"os")) ) {
		dump_nom(stem,2,"hs_eos neut");	
		return(1);
	}

	if( !strcmp("<itype>u=qos</itype>",key1) && 
	    (!strcmp(stem+strlen(stem)-3,"u/s")) ) {
		char tmpstem[BUFSIZ];
/*
 * kludge to get the long mark in over the last consonant
 */
		Xstrncpy(tmpstem,stem,sizeof(tmpstem));
		Xstrncpy(tmpstem+strlen(stem)-3,"u_/s",sizeof(tmpstem)-(strlen(stem)-3));

		dump_nom(tmpstem,2,"s_qos fem stem_acc");	
		return(1);
	}

	if( !strcmp("<itype>ews</itype>",key1) && 
	    (!strcmp(stem+strlen(stem)-2,"is")) ) {
		dump_nom(stem,2,"is_ews fem");	
		return(1);
	}

	if( (!strcmp("<itype>a</itype>",key1) || 
	     !strcmp("<itype>e/a</itype>",key1)) && 
	    (!strcmp(stem+strlen(stem)-5,"te/os")) ) {
		dump_adj(stem,4,"verb_adj2 suff_acc");	
		return(1);
	}

	if( !strcmp("<itype>a/dos</itype>",key1) && 
	    (!strcmp(stem+strlen(stem)-3,"a/s")) ) {
		dump_nom(stem,2,"s_dos fem stem_acc");	
		return(1);
	}

	if( !strcmp("<itype>i/dos</itype>",key1) && 
	    (!strcmp(stem+strlen(stem)-3,"i/s")) ) {
		dump_nom(stem,3,"is_idos fem suff_acc");	
		return(1);
	}

	if( !strcmp("<itype>ontos</itype>",key1) && 
	    !strcmp(stem+strlen(stem)-2,"wn") ) {
		dump_nom(stem,2,"wn_ontos masc");	
		return(1);
	}

	if( !strcmp("<itype>ousa</itype>",key1) && 
	    !strcmp(stem+strlen(stem)-2,"wn") ) {
		dump_adj(stem,2,"wn_ousa_on");	
		return(1);
	}

	if( !strcmp("<itype>on</itype>",key1) && 
	    !strcmp(stem+strlen(stem)-2,"wn") ) {
		dump_adj(stem,2,"wn_on");	
		return(1);
	}

	if( !strcmp("<itype>w=nos</itype>",key1) && 
	    !strcmp(stem+strlen(stem)-3,"w/n") ) {
		dump_nom(stem,1,"n_nos masc stem_acc");	
		return(1);
	}

	if( !strcmp("<itype>u/os</itype>",key1) && 
	    !strcmp(stem+strlen(stem)-3,"u/s") ) {
		dump_nom(stem,3,"us_uos fem suff_acc");	
		return(1);
	}

	if( !strcmp("<itype>wnos</itype>",key1) && 
	    !strcmp(stem+strlen(stem)-2,"wn") ) {
		dump_nom(stem,1,"n_nos masc");	
		return(1);
	}

	if( !strcmp("<itype>onos</itype>",key1) && 
	    !strcmp(stem+strlen(stem)-2,"wn") ) {
		dump_nom(stem,2,"wn_onos masc");	
		return(1);
	}

	if( !strcmp("<itype>htos</itype>",key1) && 
	    !strcmp(stem+strlen(stem)-2,"hs") ) {
		dump_nom(stem,1,"s_tos fem");	
		return(1);
	}

	if( !strcmp("<itype>ou</itype>",key1) && 
	   (!strcmp(stem+strlen(stem)-2,"as") ||
	    !strcmp(stem+strlen(stem)-2,"hs"))) {
		dump_nom(stem,2,"hs_ou masc stem_acc");	
		return(1);
	}

	if( !strcmp("<itype>ou=</itype>",key1) && 
	    !strcmp(stem+strlen(stem)-3,"h/s") ) {
		dump_nom(stem,3,"hs_ou masc suff_acc");	
		return(1);
	}

	if( !strcmp("<itype>es</itype>",key1) && 
	    !strcmp(stem+strlen(stem)-5,"w/dhs") ) {
		dump_adj(stem,2,"hs_es stem_acc");	
		return(1);
	}

	if( !strcmp("<itype>e/s</itype>",key1) && 
	    !strcmp(stem+strlen(stem)-3,"h/s") ) {
		dump_adj(stem,3,"hs_es suff_acc");	
		return(1);
	}

	if( !strcmp("<itype>o/n</itype>",key1) && 
	    !strcmp(stem+strlen(stem)-3,"o/s") ) {
		dump_adj(stem,3,"os_on suff_acc");	
		return(1);
	}

	if( (!strcmp("<itype>h/</itype>",key1) ||
	     !strcmp("<itype>a/</itype>",key1))&& 
	    !strcmp(stem+strlen(stem)-3,"o/s") ) {
		dump_adj(stem,3,"os_h_on suff_acc");	
		return(1);
	}

	if( (!strcmp("<itype>a</itype>",key1)||
		!strcmp("<itype>h</itype>",key1))  && 
	    !strcmp(stem+strlen(stem)-2,"os") ) {
		dump_adj(stem,2,"os_h_on");	
		return(1);
	}

	if( !strcmp("<itype>on</itype>",key1) && 
	    !strcmp(stem+strlen(stem)-2,"os") ) {
		dump_acc_nom(stem,2,"os_on");	
/*
		dump_adj(stem,2,"os_on");	
*/
		return(1);
	}

	if( !strcmp("<itype>atos</itype>",key1) && 
	    !strcmp(key2,"<gen>to/</gen>") ) {
		dump_nom(stem,2,"ma_matos neut");	
		return(1);
	}

	if( !strcmp("<itype>ews</itype>",key1) && 
	    !strcmp(key2,"<gen>h(</gen>") ) {
		dump_nom(stem,2,"is_ews fem");	
		return(1);
	}

	if( !strcmp("<itype>e/ws</itype>",key1) && 
	    !strcmp(stem+strlen(stem)-4,"eu/s") ) {
		dump_nom(stem,4,"eus_ews masc suff_acc");	
		return(1);
	}

	if( !strcmp("<itype>oros</itype>",key1) && 
	    !strcmp(stem+strlen(stem)-2,"wr") ) {
		dump_nom(stem,2,"wr_oros masc");	
		return(1);
	}

	if( !strcmp("<itype>h=ros</itype>",key1) && 
	    !strcmp(stem+strlen(stem)-1,"r") ) {
		dump_nom(stem,1,"r_ros masc stem_acc");	
		return(1);
	}

	return(0);
}

int is_verb(char *stem)
{
	if( !strcmp(stem+strlen(stem)-4,"eu/w") ) {
		dump_entry(stem,4,"euw",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-7,"eu/omai") ) {
		dump_entry(stem,7,"euw mp",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-4,"u/nw") ) {
		dump_entry(stem,4,"unw",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-7,"u/nomai") ) {
		dump_entry(stem,7,"unw mp",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-3,"e/w") ) {
		dump_entry(stem,3,"ew_denom",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-6,"e/omai") ) {
		dump_entry(stem,6,"ew_denom mp",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-3,"o/w") ) {
		dump_entry(stem,3,"ow_denom",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-6,"o/omai") ) {
		dump_entry(stem,6,"ow_denom mp",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-4,"u/zw") ) {
		dump_entry(stem,4,"uzw",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-7,"u/zomai") ) {
		dump_entry(stem,7,"uzw mp",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-4,"i/zw") ) {
		dump_entry(stem,4,"izw",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-7,"i/zomai") ) {
		dump_entry(stem,7,"izw mp",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-6,"ssomai") ) {
		dump_entry(stem,6,"ss mp",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-3,"ssw") ) {
		dump_entry(stem,3,"ss",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-4,"a/zw") ) {
		dump_entry(stem,4,"azw",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-7,"a/zomai") ) {
		dump_entry(stem,7,"azw mp",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-4,"ia/w") ) {
		dump_entry(stem,4,"iaw_denom",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-7,"ia/omai") ) {
		dump_entry(stem,7,"iaw_denom mp",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-3,"a/w") ) {
		dump_entry(stem,3,"aw_denom",":de:",fverbs);	
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-6,"a/omai") ) {
		dump_entry(stem,6,"aw_denom mp",":de:",fverbs);	
		return(1);
	}

	return(0);
}

int is_adverb(char *stem,char * key1)
{
	if( !strcmp(key1,"<pos>Adv.</pos>") ) {
		char tmp[BUFSIZ];


		Xstrncpy(tmp,stem,sizeof(tmp));
		standword(tmp);
		printf(":le:%s\n:wd:%s adverb\n\n", tmp, stem );
/*
		dump_entry(stem,0,"adverb",":wd:",stdout);	
*/
		return(1);
	}

	return(0);
}

void dump_acc_nom(char * stem,int trunc,char * tag)
{
	char tmptag[BUFSIZ];
	char tmpstem[BUFSIZ];
	int syllno, curacc;

	Xstrncpy(tmpstem,stem,sizeof(tmpstem));
	*(tmpstem+strlen(tmpstem)-trunc) = 0;

	checkaccent(tmpstem,&syllno,&curacc);
	if( syllno == ULTIMA ) {
		snprintf(tmptag,sizeof(tmptag),"%s stem_acc", tag );
		dump_nom(stem,trunc,tmptag);
	} else
		dump_nom(stem,trunc,tag);
}

int is_teos(char *stem)
{
	if( !strcmp(stem+strlen(stem)-5,"te/os") ) {
		dump_adj(stem,4,"verb_adj2 suff_acc");
		return(1);
	}

	if( !strcmp(stem+strlen(stem)-5,"te/on") ) {
		dump_adj(stem,4,"verb_adj2 suff_acc");
		return(1);
	}
	return(0);
}

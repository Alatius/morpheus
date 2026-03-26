#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "smk2beta.h"

#define MAXCHAR 		256
#define MAXSUBSTRING 	6
#define ROMAN 1
#define GREEK 2
#define ITALIC 3

#include "smk2beta.proto.h"
#include "../greeklib/xstrings.proto.h"
static void conv(char *, char *);
static void add_acc(char *, int);
static int smkinited = 0;
char *Xlit_table_smk[MAXCHAR+1];
char *Xlit_table_smarta[MAXCHAR+1];
char **Xlit_table;
char smarta_char[MAXCHAR+1];
static int fromsmk = 0;
static int cur_font = 0;

void smarta2beta(char *start, char *result)
{
	Xlit_table = Xlit_table_smarta;
	
	fromsmk = 0;
	
	conv(start,result);
}

void smk2beta(char *start, char *result)
{
	Xlit_table = Xlit_table_smk;

	fromsmk = 1;
	
	conv(start,result);
}

static void conv(char *start, char *result)
{
	char tmp[BUFSIZ];
	/*unsigned*/ char * s = start;
	
	cur_font =  0;
	if( !smkinited ) {
		smkinited++;
		init_smk();
	}
	
	*result = 0;
	/*
	 * make sure that any unaccented upper case char gets properly converted
	 */
	if(isupper(*s) && fromsmk) {
		Xstrncpy(tmp,s+1,sizeof(tmp));
		*(s+1) = tolower(*s);
		*s = '*';
		Xstrncpy(s+2,tmp,BUFSIZ-2);
	}
	
	while(*s) {
/*
if( Xstrlen(result) > 256 ) {
printf("hey:%s\n", result);
*(result+256 ) = 0;
return(0);
}
*/
		if( *s == '^' && ! fromsmk ) {
			s++;

			trap_upper(result,s);
			s++;
/*
			strcat(result,"*");
*/
			continue;
		}
		
		if(isupper(*s) && ! fromsmk ) {
			if( cur_font == GREEK || ! cur_font ) 
				set_cur_font(ROMAN,result);
			tmp[0] = tolower(*s);
			tmp[1] = 0;
			Xstrncat(result,tmp,BUFSIZ);
			s++;
			continue;
		}
		if( *s == '_' && ! fromsmk ) {
			set_cur_font(ITALIC,result);
			s++;
			continue;
		}
		if( *s == '\xab' && ! fromsmk ) {
			set_cur_font(ROMAN,result);
			s++;
			continue;
		}
		if(*s == '`' && ! fromsmk ) {
			if( ! cur_font || cur_font == GREEK ) {
				set_cur_font(ROMAN,result);
			}
			Xstrncat(result,":",BUFSIZ);
			s++;
			continue;
		}
		
		if( (*s & 0377) >= 0202 && (*s & 0377) <= 0212 ) {
			set_cur_font(GREEK,result);
			Xstrncat(result,Xlit_table[(int)(*s++ & (0377))],BUFSIZ);

			/*
			 * we should only have an accented space (0202 thru 0212)
			 * if we have an upper case vowel following. in this case,
			 * we want to convert to a slighly different sequence for
			 * beta.
			 *
			 * something like "\0205A" would be "*)/a" in beta
			 * transliteration
			 */
			if( isupper(*s) ) {
				tmp[0] = tolower(*s);
			} else
				tmp[0] = *s;
			tmp[0] = smk2betachar(tmp[0]);
			tmp[1] = 0;
			Xstrncat(result,tmp,BUFSIZ);
			s++;	
		} else {
			if( ! fromsmk ) {
				if( (smarta_char[(int)(*s & (0377))] || islower(*s)) && 
				  (cur_font == ROMAN || cur_font == ITALIC || ! cur_font ) )
					set_cur_font(GREEK,result);	
			}
			Xstrncat(result,Xlit_table[(int)(*s++ & (0377))],BUFSIZ);
		}
	}
}

int smk2betachar(int c)
{
	if( c == 'v' ) return('w');
	if( c == 'y' ) return('q');
	if( c == 'c' ) return('y');
	if( c == 'j' ) return('c');
	if( c == 'W' ) return('v');
	return(c);
}

void init_smk(void)
{
	int i;
	size_t j;

	for(i=0;i<MAXCHAR;i++) {
		Xlit_table_smk[i] = malloc((size_t)MAXSUBSTRING);
		*Xlit_table_smk[i] = 0;

		Xlit_table_smarta[i] = malloc((size_t)MAXSUBSTRING);
		*Xlit_table_smarta[i] = 0;
	}

	for(j=0;j<sizeof Beta_SMK/sizeof Beta_SMK[0];j++) {
		Xstrncpy(Xlit_table_smk[Beta_SMK[j].keycode],Beta_SMK[j].keystring,MAXSUBSTRING);
		Xstrncpy(Xlit_table_smarta[Beta_SMK[j].keycode],Beta_SMK[j].keystring,MAXSUBSTRING);
		smarta_char[Beta_SMK[j].keycode] =1;
/*
printf("%d) Xlit_table [%s]\n", Beta_SMK[j].keycode , Xlit_table[Beta_SMK[j].keycode] );
*/
	}
	for(j=0;j<sizeof Beta_Smarta/sizeof Beta_Smarta[0];j++) {
		Xstrncpy(Xlit_table_smarta[Beta_Smarta[j].keycode],Beta_Smarta[j].keystring,MAXSUBSTRING);
	}
	
	for(i=0;i<256;i++) {
		if( ! *Xlit_table_smk[i] ) {
			snprintf(Xlit_table_smk[i] , sizeof(Xlit_table_smk[i]), "%c", i );
		}
		if( ! *Xlit_table_smarta[i] ) {
			snprintf(Xlit_table_smarta[i] , sizeof(Xlit_table_smarta[i]), "%c", i );
		}
	}

}

void set_cur_font(int n, char *s)
{
	if( fromsmk ) return;
	
	if( n != cur_font ) {
		switch(n) {
			case GREEK:
				Xstrncat(s,"$",BUFSIZ);
				break;
			case ROMAN:
				Xstrncat(s,"&",BUFSIZ);
				break;
			case ITALIC:
				Xstrncat(s,"&3",BUFSIZ);
				break;
			default:
				Xstrncat(s,"?Font?",BUFSIZ);
				break;
			}
		cur_font = n;
		}
}
#define SPACE_ACUTE 0200
#define ALPHA_ACUTE 0213
#define EPSILON_ACUTE 0241
#define IOTA_ACUTE 0333
#define OMICRON_ACUTE 0361	
#define UPSILON_ACUTE 0346
#define ETA_ACUTE 0256
#define WMEGA_ACUTE 0305
#define AISUB_ACUTE 0226
#define EISUB_ACUTE 0372
#define WISUB_ACUTE 0304

void trap_upper(char *res, char *s)
{
	char tmp[BUFSIZ];

	if( isupper(*s) ) {
		if( !cur_font || cur_font == GREEK ) {
			set_cur_font(ROMAN,res);
		}

		tmp[0] = '*';
		tmp[1] = tolower(*s);
		tmp[2] = 0;
		Xstrncat(res,tmp,BUFSIZ);
		return;
	}

	if( islower(*s) ) {
		if( !cur_font || cur_font == ROMAN || cur_font == ITALIC ) {
			set_cur_font(GREEK,res);
		}
		tmp[0] = '*';
		tmp[1] = *s;
		tmp[2] = 0;
		Xstrncat(res,tmp,BUFSIZ);
		return;
	}

	tmp[0] = 0;
	if( SMK_ALPHA(*s) ) {
		add_acc(tmp, *s - ALPHA_ACUTE + SPACE_ACUTE);
		Xstrncat(tmp,"a",sizeof(tmp));
	} else if( SMK_EPSILON(*s) ) {
		add_acc(tmp, *s - EPSILON_ACUTE + SPACE_ACUTE);
		Xstrncat(tmp,"e",sizeof(tmp));
	} else if( SMK_IOTA(*s) ) {
		add_acc(tmp, *s - IOTA_ACUTE + SPACE_ACUTE);
		Xstrncat(tmp,"i",sizeof(tmp));
	} else if( SMK_OMICRON(*s) ) {
		add_acc(tmp, *s - OMICRON_ACUTE + SPACE_ACUTE);
		Xstrncat(tmp,"o",sizeof(tmp));
	} else if( SMK_UPSILON(*s) ) {
		add_acc(tmp, *s - UPSILON_ACUTE + SPACE_ACUTE);
		Xstrncat(tmp,"u",sizeof(tmp));
	} else if( SMK_ETA(*s) ) {
		add_acc(tmp, *s - ETA_ACUTE + SPACE_ACUTE);
		Xstrncat(tmp,"h",sizeof(tmp));
	} else if( SMK_WMEGA(*s) ) {
		add_acc(tmp, *s - WMEGA_ACUTE + SPACE_ACUTE);
		Xstrncat(tmp,"w",sizeof(tmp));
	} else if( SMK_AISUB(*s) ) {
		add_acc(tmp, *s - AISUB_ACUTE + SPACE_ACUTE);
		Xstrncat(tmp,"_",sizeof(tmp));
		Xstrncat(tmp,"a",sizeof(tmp));
	} else if( SMK_EISUB(*s) ) {
		add_acc(tmp, *s - EISUB_ACUTE + SPACE_ACUTE);
		Xstrncat(tmp,"_",sizeof(tmp));
		Xstrncat(tmp,"h",sizeof(tmp));
	} else if( SMK_WISUB(*s) ) {
		add_acc(tmp, *s - WISUB_ACUTE + SPACE_ACUTE);
		Xstrncat(tmp,"_",sizeof(tmp));
		Xstrncat(tmp,"w",sizeof(tmp));
	}
	if( tmp[0] ) {
		if( ! cur_font || cur_font == ROMAN || cur_font == ITALIC )
			set_cur_font(GREEK,res);
		Xstrncat(res,tmp,BUFSIZ);
	}
}

static void add_acc(char *s, int anum)
{
	Xstrncpy(s,Xlit_table[(int)( anum & (0377))],BUFSIZ);
}

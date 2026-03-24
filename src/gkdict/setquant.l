%option noyywrap main

%{
#include <ctype.h>
#include <string.h>
void set_quant(void);
void set_quant2(void);
int quantcmp(char *want, char *got, int curquant);
int is_match(char *want, char *got, int curquant);
void addquant(char *s, int c);
%}

%%


[a-z\\\/\=\(\)\+\|\'\-\_\^\*]+[ \t]+"<quant>["[a-z\^\_]+"]</quant>" set_quant2();


%%
/*
[a-z\\\/\=\(\)\+\|\'\-\_\^\*]+[ \t]+"<quant>"[^<]+ set_quant();
*/

#include <greek.h>

void set_quant()
{
	char cmps[BUFSIZ];
	char * s;
	int curquant = 0;

	s = yytext+yyleng-1;

	while(s>yytext) {
		if(*s == '^' || *s == '_' ) {
			curquant = *s;
			break;
		}
		s--;
	}
printf("%s curq [%c]\n", yytext , curquant);
}

void set_quant2()
{
	char subs[BUFSIZ];
	int curquant = 0;
	char curw[BUFSIZ], *s;


	s = yytext+yyleng-strlen("]</quant>");
	while(s>=yytext) {
		if(Is_quant(*s)) {
			curquant = *s;
			break;
		}
		s--;
	}
	while(s>=yytext&&*s!='[') s--; 
	if(*s == '[' ) {
		strcpy(subs,s+1);
		s = strchr(subs,']');
		if(s) *s = 0;
	} else ECHO;
	
	strcpy(curw,yytext);
	s = curw;
	while(*s&&!isspace(*s)) s++;
	if(*s&&isspace(*s)) *s = 0;
/*
	printf("[%s] %c %s \n", curw, curquant, subs );
*/
	if( quantcmp(subs,curw, curquant) ) printf("%s", curw );
	else ECHO;
}

int quantcmp(char *want,char * got,int curquant)
{
	while(*got) {
		if( is_match(want,got,curquant) ) {
			addquant(got,curquant);
			return(1);
		} /*else printf("failed! [%s] [%s]\n", got , want ); */
		got++;
		while(Is_diacrit(*got)) got++;
	}
	return(0);
}

int is_match(char *want, char *got,int curquant)
{
	int sawdiac  = 0;

	while(*want) {
		while(Is_diacrit(*want)) want++;
		if(Is_diacrit(*got)) {
			sawdiac++;
			while(Is_diacrit(*got)) got++;
		}
		if(*want!=*got) {
			return(0);
		} 
		got++; want++;
		while(Is_diacrit(*want)) want++;
	}
	if( (! *want) && (sawdiac||Is_diacrit(*got))) return(1);

	return(0);
}

void addquant(char *s, int c)
{
	char tmp[BUFSIZ];

	while(*s&&!Is_diacrit(*s)) s++;
	while(Is_diacrit(*s)) s++;
	strcpy(tmp,s);
	*s++ = c;
	strcpy(s,tmp);
}

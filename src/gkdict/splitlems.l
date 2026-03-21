%option noyywrap main

%{
void set_lemma(void);
void dump_orth(void);
void add_lemma(void);
%}

%%

^[A-Za-z\^\_\-]+[#1-9]* 	set_lemma();

\t[a-z\*] printf("\n%s", yytext+1);
"</gen>\t<orth>"[^<]+"</orth>"	dump_orth();
"</pos>\t<orth>"[^<]+"</orth>"	dump_orth();
"</itype>\t<orth>"[^<]+"</orth>"	dump_orth();
"</gen>\t<itype>"	add_lemma();

%%

#include <string.h>
unsigned char curlemma[BUFSIZ];

void add_lemma()
{
printf("</gen>\n%s\t<itype>", curlemma );
}

void set_lemma()
{
	ECHO;
	strcpy(curlemma,yytext);
}

void dump_orth()
{
	unsigned char * t;
	yytext[yyleng-7] = 0;
	
	t = yytext;
	while(*t&&*t!='\t') putchar(*t++);
	if(*t=='\t') putchar('\n');

	while(*t&&*t!='>') t++;
	if(*t=='>') t++;
	printf("%s\t", t );
}


%option noyywrap

%%

"key=\""[^"]+	getkey();

. |
\n	;
%%

getkey()
{
	printf("%s\n", yytext+5 );
}

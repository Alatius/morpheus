%option noyywrap main nounput noinput

%%

"&"[aeiou]"macr;"	printf("%c_", yytext[1] );
"&euml;"	printf("e+");
"&uuml;"	printf("u+");
"&iuml;"	printf("i+");

%%

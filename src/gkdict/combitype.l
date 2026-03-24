%option noyywrap main nounput noinput

%%

"</itype>"[ \t]+"<itype>"	printf(", ");

%%

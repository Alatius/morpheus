%option noyywrap main

%%

"</itype>"[ \t]+"<itype>"	printf(", ");

%%

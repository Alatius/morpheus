#include <stdio.h>
#include "y.tab.h"
#include <string.h>
#include "../greeklib/xstrings.proto.h"

char line[10000];
char anals[10000];

yylex()
{

	while(fgets(line, sizeof line, stdin)) {
		line[strcspn(line, "\n")] = '\0';
		if(!fgets(anals, sizeof anals, stdin)) break;
		anals[strcspn(anals, "\n")] = '\0';
		passVal(line);
		if(is_substring(" article",anals)) return ARTICLE;
		if(is_substring("<NL>N",anals)) return(NOUN);
		if(is_substring("<NL>P",anals)) return(NOUN);
		if(is_substring(" prep",anals)) return(PREP);
	}	
	return(0);
}


passVal()
{
        yylval.string = (char *)malloc( (strlen(line) + 1)  );
        if (!yylval.string)
                fprintf(stderr, "Out of memory for %s\n", line );
        else
                strncpy( yylval.string, line, sizeof(line) );
                yylval.string[sizeof(line) - 1] = '\0';
/*        fprintf(stderr, "%s\n",yylval.string);*/
}


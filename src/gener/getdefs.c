#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../greeklib/xstrings.proto.h"

void _main()
{
		char line[BUFSIZ], *s;
		char curlemma[BUFSIZ];
		char number[80], defstr[BUFSIZ*4];
		curlemma[0] = 0;
		
		while(fgets(line, sizeof line, stdin)) {
			line[strcspn(line, "\n")] = '\0';
			if( !strncmp(line,":le:",4) ) {
				Xstrncpy(curlemma,line+4,sizeof curlemma);
				s = curlemma+strlen(curlemma)-1;
				while(isspace(*s) && s>curlemma) *s-- = 0;
				continue;
			}
			if(line[0] !=':' && line[0] != '?' && line[0] != ';' && line[0] != '@' ) {
				if( line[0] == '[' ) {
					Xstrncpy(number,line+1,sizeof number);
					s = line+1;
					while(*s && *s!=']') s++;
					if( *s )
						*s++ = 0;
				} else {
					s = line;
					Xstrncpy(number,"0",sizeof number);
				}
				while(*s && isspace(*s)) s++;
				Xstrncpy(defstr,s,sizeof defstr);
				printf("%s\t%s\t%s\n", curlemma, number, defstr );
			}
		}
		
}
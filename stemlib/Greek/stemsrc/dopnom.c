#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(void)
{
	char line[BUFSIZ];

	while(fgets(line, sizeof line, stdin)) {
		line[strcspn(line, "\n")] = 0;
		char *s;
		if(strncmp(line,":ns:",4)) continue;
		s = line+4;
		printf(":le:");
		while(*s&&!isspace(*s)) putchar(*s++);
		putchar('\n');
		while(isspace(*s)) s++;
		if( strncmp(s,":wd:",4) ) printf(":no:");
		printf("%s\n\n"	, s);

	}
	return 0;
}

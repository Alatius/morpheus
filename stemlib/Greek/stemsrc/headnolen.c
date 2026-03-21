#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(void)
{
	char line[BUFSIZ];
	char * s;

	while(fgets(line, sizeof line, stdin)) {
		line[strcspn(line, "\n")] = 0;
		s = line;

		while(*s&&!isspace(*s)) {
			if( *s != '_' && *s!= '^' ) putchar(*s);
			s++;
		}
		printf(" %s\n",line );
	}
	return 0;
}


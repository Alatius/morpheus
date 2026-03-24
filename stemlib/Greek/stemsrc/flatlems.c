#include <stdio.h>
#include <ctype.h>
#include <string.h>

char curlem[BUFSIZ];

int main(void)
{
	char line[BUFSIZ*10];
	while(fgets(line, sizeof line, stdin)) {
		line[strcspn(line, "\n")] = 0;
		if( !strncmp(":le:",line,4)) {
			strncpy(curlem, line+4, sizeof(curlem)); curlem[sizeof(curlem)-1] = '\0';
			continue;
		}
		if( !strncmp(":no:",line,4) ||  !strncmp(":aj:",line,4)) {
			printf("%s\t%s\n", curlem, line );
		}
	}
	return 0;
}

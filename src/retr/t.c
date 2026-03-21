#include <stdio.h>
#include <string.h>

char * is_common();

main()
{
	char line[128];
	char * p;

	while(fgets(line, sizeof line, stdin))  {
		line[strcspn(line, "\n")] = '\0';
		p=is_common(line);
		if(p) printf("got [%s]\n", p );
		else printf("failed on [%s]\n", line );
	}
}

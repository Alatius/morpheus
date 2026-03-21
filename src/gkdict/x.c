#include <stdio.h>
#include <string.h>

main()
{
	char line[200];

	while(fgets(line, sizeof line, stdin)) {
		line[strcspn(line, "\n")] = '\0';
		putsimpleacc(line);	
		printf("%s\n", line );
	}
}

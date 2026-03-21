#include <stdio.h>
#include <string.h>
#include <prntflags.h>
int quickflag = 0;

main()
{
	char line[BUFSIZ];
	int n;

	while(fgets(line, sizeof line, stdin)) {
		line[strcspn(line, "\n")] = '\0';
		n=checkstring(line,(PrntFlags) LEMCOUNT,stdout);
		printf("rval for [%s] is [%d]\n", line, n );
	}
}

#include <stdio.h>
#include <string.h>

#include "index dict.proto.h"

main(void)
{
	char line[256];
	char stemkeys[256];
	int rval;

	printf("dictionary? ");
	fgets(line, sizeof line, stdin);
	line[strcspn(line, "\n")] = '\0';
	printf("munch %s\n", line );
	rval = index_list(line,":le:");
	printf("done indexing\n");
}
	
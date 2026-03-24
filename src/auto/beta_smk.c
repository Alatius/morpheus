#include <stdio.h>
#include "beta_smk.h"
#include "../greeklib/xstrings.proto.h"

main()
{
	int c;
	char line[256];
	char result[256];

	while((c=getchar()) != EOF ) {
		if( c == '\r' ) {
			putchar('\n');
			continue;
		}
		smk2beta(c,result);
		printf("%s", result );
	}
}

static int smkinited = 0;
char Xlit_table[257][6];

smk2beta(c,result)
int c;
char * result;
{
	if( !smkinited ) {
		smkinited++;
		init_smk();
	}
	strncpy(result, Xlit_table[c], sizeof(Xlit_table[c]));
	result[sizeof(Xlit_table[c]) - 1] = '\0';
}

init_smk()
{
	int i;
	char tmp[80];

	for(i=0;i<sizeof Beta_SMK/sizeof Beta_SMK[0];i++) {
		strncpy(Xlit_table[Beta_SMK[i].keycode], Beta_SMK[i].keystring, sizeof(Xlit_table[0]));
		Xlit_table[Beta_SMK[i].keycode][sizeof(Xlit_table[0]) - 1] = '\0';
printf("%d) Xlit_table [%s]\n", Beta_SMK[i].keycode , Xlit_table[Beta_SMK[i].keycode] );
	}
		
	for(i=0;i<256;i++) {
		if( ! Xlit_table[i][0] )
			snprintf(Xlit_table[i] , sizeof(Xlit_table[i]), "%c", i );
	}

}

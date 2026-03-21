#include <gkstring.h>

gk_word GkWord, BlnkWord;
gk_string Gstr, AvoidGstr, BlnkGstr;

main()
{
	char line[2000];

	while(fgets(line, sizeof line, stdin)) {
		line[strcspn(line, "\n")] = '\0';
		ScanAsciiKeys((char *)line,&GkWord,&Gstr,&AvoidGstr);
		printf("stemtype Gkword %o Gstr %o\n", stemtype_of(&GkWord), stemtype_of(&Gstr) );
		Gstr = AvoidGstr = BlnkGstr;
		GkWord = BlnkWord;
	}
}

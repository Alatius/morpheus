/*
 * grc 11/28/94
 *
 * make all long alpha-s into etas -- used to look for words that
 * are the same except for minor dialectical shift between alpha and eta
 */


#include <string.h>

void standalpha(char *s )
{
	while(*s) {
		if(!strncmp("a_",s,2)) {
			*s++ = 'h';
			memmove(s,s+1,strlen(s+1)+1);
		}
		s++;
	}

}

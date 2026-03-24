#include <gkstring.h>
#include <unistd.h>

#include "indexstems.main.proto.h"
#include "indexstems.proto.h"
#include "../morphlib/setlang.proto.h"
int verbose = 0;


int main(int argc, char *argv[])
{
        int c, errflg = 0;

        while (!errflg && (c = getopt(argc,argv,"IL")) != -1) {
                switch (c) {
                        case 'I':
                                set_lang(ITALIAN);
                                break;
                        case 'L':
                                set_lang(LATIN);
                                break;
                        default:
                                break;
                }
        }

	index_vbs(3,0,0);

	return 0;
}

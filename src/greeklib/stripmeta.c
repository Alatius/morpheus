
#include "stripmeta.proto.h"
/*
 * gregory crane
 *  
 * november 1987
 */
 
void stripmetachars(char *s)
{
	stripquant(s);
	stripzeroend(s);
	stripstemsep(s);
	zap_rr_breath(s);
}

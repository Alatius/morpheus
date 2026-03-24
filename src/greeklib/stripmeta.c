
#include "stripmeta.proto.h"
#include "stripquant.proto.h"
#include "stripstemsep.proto.h"
#include "stripzeroend.proto.h"
#include "zap2ndbreath.proto.h"
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

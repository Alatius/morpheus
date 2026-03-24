#include <gkstring.h>
#include "../greeklib/xstrings.proto.h"

int set_sampleform(char *keys)
{
	word_form wf;
	gk_string * gstr;
	gk_string * avoidgstr;
	
	gstr = CreatGkString(1);
	avoidgstr = CreatGkString(1);
	
	ScanAsciiKeys(keys,gstr,avoidgstr,NULL,NULL);
	wf = forminfo_of(avoidgstr);
	if( person_of(wf) || number_of(wf) || mood_of(wf) || tense_of(wf) || voice_of(wf) || case_of(wf) )
		return(0);
		
	wf = forminfo_of(gstr);
	
	if( Is_verbform(gstr) ) {
		if( ! person_of(wf) ) {
			Xstrncat(keys," 1st",BUFSIZ);
		if( ! number_of(wf) )
			Xstrncat(keys," sing",BUFSIZ);
		if( ! mood_of(wf) )
			Xstrncat(keys," ind",BUFSIZ);
		if( ! voice_of(wf) )
			Xstrncat(keys," act",BUFSIZ);
	}
	if( ! tense_of(wf) ) {
			switch(stemtype_of(gstr) ) {
				case W_STEM:
				case EW_PR:
				case AW_PR:
				case OW_PR:
				case EMI_PR:
				case AMI_PR:
				case OMI_PR:
				case UMI_PR:
					Xstrncat(keys," pres",BUFSIZ);
					break;
				case AOR_PASS:
				case AOR2_PASS:
				case AOR1:
				case AOR2:
				case EMI_AOR:
				case AMI_AOR:
				case OMI_AOR:
				case UMI_AOR:
					Xstrncat(keys," aor",BUFSIZ);
					break;
				case	PERF_ACT:
				case	PERF2_ACT:
				case	PERFP_G:
				case	PERFP_GX:
				case	PERFP_L:
				case	PERFP_MP:
				case	PERFP_N:
				case	PERFP_P:
				case	PERFP_S:
				case	PERFP_VOW:
				case	PERF_ACT:
				case	PERFP_R:
				case PERF_ACT:
					Xstrncat(keys," perf",BUFSIZ);
					break;
			    default:
			    		break;
			    }
		}
}

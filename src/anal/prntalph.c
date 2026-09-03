#include <gkstring.h>
#include "prntalph.h"
#include "prntanal.proto.h"
#include "../morphlib/morphflags.proto.h"
#include "../morphlib/setlang.proto.h"

typedef struct
{
	const char*	d_name;
	long		d_flags;
} MorphEntry;

typedef struct
{
	const char*	d_name;
	const char*	d_value;
} AttributeEntry;

/*
	Tables mapping flags for morphological categories to textual names
	Note: Entries are tested by ANDing flags with table entries, and
	some entries correspond to flag combinations.  Entries are tested
	in order until a match is found, so entries with multiple flag
	values must appear in the table after values with fewer flags.
 */

/*
#define	SUBMASK	0777
MorphEntry	alpheiosPofsNames[] =
{
	{"adjective",	ADJSTEM|SUBMASK},
	{"noun",		NOUNSTEM|SUBMASK},
	{"verb",		PPARTMASK},
	{"verb",		VERBSTEM},
	{"numeral",		NUMERAL},
	{"preposition",	PREPOSITION},
	{"article",		ARTICLE},
	{"pronoun",		PRONOUN},
	{"pronoun",		INDEF_PRON},
	{"pronoun",		PERS_PRON},
	{"pronoun",		REL_PRON},
	{"pronoun",		INDEF_REL_PRON},
	{"particle",	PARTICLE},
	{"conjunction",	CONJUNCT},
	{"adverb",		ADVERB},
	{NULL,			0}
};
*/

AttributeEntry	alpheiosPofsOrder[] =
{
	{"adverb",		"7"},
	{"preposition",	"6"},
	{"pronoun",		"5"},
	{"numeral",		"4"},
	{"noun",		"3"},
	{"adjective",	"2"},
	{"verb",		"1"},
	{NULL,			"0"}
};

MorphEntry	alpheiosDeclNames[] =
{
	{"1st",	DECL1},
	{"2nd",	DECL2},
	{"3rd",	DECL3},
	{"4th",	DECL4},
	{"5th",	DECL5},
	{"1st & 2nd", DECL1|DECL2},
	{"1st & 3rd", DECL1|DECL3},
	{NULL,	0}
};

MorphEntry	alpheiosCaseNames[] =
{
	{"nominative",						NOMINATIVE},
	{"genitive",						GENITIVE},
	{"ablative",						ABLATIVE},
	{"dative",							DATIVE},
	{"accusative",						ACCUSATIVE},
	{"vocative",						VOCATIVE},
	{"genitive/dative",					GENITIVE|DATIVE},
	{"ablative/dative",					ABLATIVE|DATIVE},
	{"nominative/accusative",			NOMINATIVE|ACCUSATIVE},
	{"nominative/vocative",				NOMINATIVE|VOCATIVE},
	{"nominative/vocative/accusative",	NOMINATIVE|VOCATIVE|ACCUSATIVE},
	{NULL,								0}
};

AttributeEntry	alpheiosCaseOrder[] =
{
	{"nominative",	"7"},
	{"genitive",	"6"},
	{"dative",		"5"},
	{"accusative",	"4"},
	{"ablative",	"3"},
	{"locative",	"2"},
	{"vocative",	"1"},
	{NULL,			"0"}
};

MorphEntry	alpheiosComparisonNames[] =
{
	{"comparative",	COMPARATIVE},
	{"superlative",	SUPERLATIVE},
	{NULL,			0}
};

MorphEntry	alpheiosGenderNames[] =
{
	{"masculine",					MASCULINE},
	{"feminine",					FEMININE},
	{"neuter",						NEUTER},
	{"adverbial",					ADVERBIAL},
	{"masculine/neuter",			MASCULINE|NEUTER},
	{"masculine/feminine",			MASCULINE|FEMININE},
	{"masculine/feminine/neuter",	MASCULINE|FEMININE|NEUTER},
	{"common",						MASCULINE|FEMININE|NEUTER},
	{NULL,							0}
};

MorphEntry	alpheiosMoodNames[] = {
	{"indicative",	INDICATIVE},
	{"subjunctive",	SUBJUNCTIVE},
	{"imperative",	IMPERATIVE},
	{"supine",		SUPINE},
	{"optative",	OPTATIVE},
	{"infinitive",	INFINITIVE},
	{"participle",	PARTICIPLE},
	{"conditional",	CONDITIONAL},
	{"gerundive",	GERUNDIVE},
	{NULL,			0}
};

MorphEntry	alpheiosNumberNames[] =
{
	{"singular",	SINGULAR},
	{"plural",		PLURAL},
	{"dual",		DUAL},
	{NULL,			0}
};

MorphEntry	alpheiosPersonNames[] =
{
	{"1st",	PERS1},
	{"2nd",	PERS2},
	{"3rd",	PERS3},
	{NULL,	0}
};

MorphEntry	alpheiosTenseNames[] =
{
	{"present",			PRESENT},
	{"future",			FUTURE},
	{"aorist",			AORIST},
	{"perfect",			PERFECT},
	{"imperfect",		IMPERF},
	{"pluperfect",		PLUPERF},
	{"future perfect",	FUTPERF},
	{"past absolute",	PASTABSOLUTE},
	{NULL,				0}
};

MorphEntry	alpheiosVoiceNames[] =
{
	{"active",			ACTIVE},
	{"passive",			PASSIVE},
	{"middle",			MIDDLE},
	{"mediopassive",	MEDIO_PASS},
	{"deponent",		ACTIVE|MIDDLE},
	{NULL,				0}
};

MorphEntry	alpheiosStemNames[] =
{
	{"pp_pr",		PP_PR},
	{"pp_fu",		PP_FU},
	{"pp_ao",		PP_AO},
	{"pp_pf",		PP_PF},
	{"pp_pp",		PP_PP},
	{"pp_ap",		PP_AP},
	{"pp_fp",		PP_FP},
	{"pp_p4",		PP_SU},
	{"pp_va",		PP_VA},
	{"pp_vn",		PP_VN},
	{"verbstem",	VERBSTEM},
	{"indecl",		INDECL},
	{"adj3",		ADJSTEM|DECL3},
	{"noun1",		NOUNSTEM|DECL1},
	{"noun2",		NOUNSTEM|DECL2},
	{"noun3",		NOUNSTEM|DECL3},
	{"noun4",		NOUNSTEM|DECL4},
	{"noun5",		NOUNSTEM|DECL5},
	{"prim_deriv",	VERBSTEM|PRIM_CONJ},
	{"reg_deriv",	VERBSTEM|REG_CONJ},
	{"adj1",		ADJSTEM|DECL1|DECL2},
	{"adj2",		ADJSTEM|DECL1|DECL2},
	{"indecl1",		INDECL|NOUNSTEM|DECL1},
	{"indecl2",		INDECL|NOUNSTEM|DECL2},
	{"indecl3",		INDECL|NOUNSTEM|DECL3},
	{"pron3",		INDECL|NOUNSTEM|DECL3},
	{"pron1",		INDECL|NOUNSTEM|DECL1|DECL2},
	{NULL,			0}
};

MorphEntry	alpheiosFlagNames[] =
{
	{"syll_augment",	SYLL_AUGMENT},
	{"comp_only",		COMP_ONLY},
	{"not_in_comp",		NOT_IN_COMPOSITION},
	{"enclitic",		ENCLITIC},
	{"proclitic",		PROCLITIC},
	{"iterative",		ITERATIVE},
	{"ant_acc",			ANT_ACC},
	{"stem_acc",		STEM_ACC},
	{"pen_acc",			STEM_ACC},
	{"suff_acc",		SUFF_ACC},
	{"ult_acc",			SUFF_ACC},
	{"rec_acc",			REC_ACC},
	{"needs_acc",		NEEDS_ACCENT},
	{"contracted",		CONTRACTED},
	{"uncontr_end",		UNCONTR_END},
	{"uncontracted",	UNCONTR_END},
	{"uncontr_stem",	UNCONTR_STEM},
	{"pers_name",		PERS_NAME},
	{"prevb_augment",	PREVB_AUGMENT},
	{"double_augment",	DOUBLE_AUGMENT},
	{"no_comp",			NO_COMP},
	{"irreg_comp",		IRREG_COMP},
	{"irreg_superl",	IRREG_SUPERL},
	{"short_pen",		SHORT_PEN},
	{"long_pen",		LONG_PEN},
	{"r_e_i_alpha",		R_E_I_ALPHA},
	{"unaugmented",		UNAUGMENTED},
	{"apocope",			APOCOPE},
	{"has_augment",		HAS_AUGMENT},
	{"nu_movable",		NU_MOVABLE},
	{"interv_s_to_h",	INTERV_S_TO_H},
	{"poetic",			POETIC},
	{"dissimilation", 	DISSIMILATION},
	{"metathesis", 		METATHESIS},
	{"elide_preverb",	ELIDE_PREVERB},
	{"root_preverb",	ROOT_PREVERB},
	{"diminutive", 		DIMINUTIVE},
	{"early", 			EARLY},
	{"late", 			LATE},
	{"rare", 			RARE},
	{"raw_preverb", 	RAW_PREVERB},
	{"short_subj", 		SHORT_SUBJ},
	{"unasp_preverb", 	UNASP_PREVERB},
	{"redupl", 			REDUPL},
	{"attic_redupl", 	ATTIC_REDUPL},
	{"is_deriv", 		IS_DERIV},
	{"no_redupl", 		NO_REDUPL},
	{"n_infix", 		N_INFIX},
	{"syncope", 		SYNCOPE},
	{"impersonal", 		IMPERSONAL},
	{"indeclform",		INDECLFORM},
	{"needs_rbreath",	NEEDS_RBREATH},
	{"no_circumflex",	NO_CIRCUMFLEX},
	{"causal",			CAUSAL},
	{"intrans",			INTRANS},
	{"tmesis",			TMESIS},
	{"raw_sonant",		RAW_SONANT},
	{"prodelision",		PRODELISION},
	{"frequentative",	FREQUENTAT},
	{"desiderative",	DESIDERATIVE},
	{"impersonal",		IMPERSONAL},
	{"later",			LATER},
	{"double_redupl",	DOUBLE_REDUPL},
	{"pres_redupl",		PRES_REDUPL},
	{"ends_in_digamma",	ENDS_IN_DIGAMMA},
	{"geog_name",		GEOG_NAME},
	{"doubled_cons",	DOUBLED_CONS},
	{"iota_intens",		IOTA_INTENS},
	{"sig_to_ci",		SIG_TO_CI},
	{"short_eis",		SHORT_EIS},
	{"pros_to_poti",	PROS_TO_POTI},
	{"pros_to_proti",	PROS_TO_PROTI},
	{"meta_to_peda",	META_TO_PEDA},
	{"upo_to_upai",		UPO_TO_UPAI},
	{"para_to_parai",	PARA_TO_PARAI},
	{"uper_to_upeir",	UPER_TO_UPEIR},
	{"en_to_eni",		EN_TO_ENI},
	{"a_priv",			A_PRIV},
	{"a_copul",			A_COPUL},
	{"metrical_long",	METRICAL_LONG},
	{NULL,				0}
};

/* dialects */
MorphEntry	alpheiosDialectNames[] =
{
	{"Attic",				ATTIC},
	{"epic",				EPIC},
	{"Homeric",				HOMERIC},
	{"non-Homeric epic",	NON_HOMERIC_EPIC},
	{"Doric",				DORIC},
	{"Ionic",				IONIC},
	{"Aeolic",				AEOLIC},
	{"paradigm form",		PARADIGM},
	// {"all",					ALL_DIAL}, commented out to represent unrestricted dialect by no dialect flags
	{"need_not_aug",		HOMERIC},
	{"prose",				PROSE},
/*
	{"eo_ou_dial",			ATTIC},
	{"laconian",			LACONIAN},
*/
	{"Ionic/Homeric",		IONIC|HOMERIC},
/*
	{"eo_eu_dial",			IONIC|HOMERIC},
	{"no_contr_fut",		HOMERIC|IONIC},
	{"a_no_contr",			HOMERIC|DORIC},
	{"ee_ee_dial",			(~ATTIC)},
	{"eo_eo_dial",			(~ATTIC)},
	{"eou_eou_dial",		(~ATTIC)},
	{"ew_ew_dial",			(~ATTIC)},
	{"un_contr",			(~ATTIC)},
*/
	{NULL,					0}
};

/* Geographical Regions */
MorphEntry	alpheiosGeoNames[] =
{
	{"Phocis",		PHOCIS},
	{"Locris",		LOCRIS},
	{"Elis",	  	ELIS},
	{"Laconia",		LACONIA},
	{"Heraclea",	HERACLEA},
	{"Megarid",		MEGARID},
	{"Argolid",		ARGOLID},
	{"Rhodes",		RHODES},
	{"Cos",			COS},
	{"Thera",		THERA},
	{"Cyrene",		CYRENE},
	{"Crete",		CRETE},
	{"Arcadia",		ARCADIA},
	{"Cyprus",		CYPRUS},
	{"Boeotia",		BOEOTIA},
	{NULL,			0}
};

const char *get_xml_lang(void)
{
    if (cur_lang() == GREEK) return "grc-x-beta";
    if (cur_lang() == LATIN) return "lat";
    if (cur_lang() == ITALIAN) return "it";
    return "und";
}

void xml_write_text(FILE *f, const char *s)
{
    if (!s) return;
    for (; *s; ++s)
    {
        switch (*s)
        {
            case '&': fputs("&amp;", f); break;
            case '<': fputs("&lt;", f); break;
            case '>': fputs("&gt;", f); break;
            case '"': fputs("&quot;", f); break;
            default:  fputc((unsigned char)*s, f); break;
        }
    }
}

void		alpheiosDumpWord(gk_word* gkword, PrntFlags prntflags, FILE* fout);
void		alpheiosDumpAnalysis(gk_analysis* analysis, FILE* fout);
const char*	alpheiosDumpPartOfSpeech(gk_analysis* analysis,
									 FILE* fout,
									 int nopart);
void		alpheiosDumpMorphology(word_form a_wf, FILE* a_fout);
void		alpheiosDumpString(const char* a_label,
							   const char* a_indent,
							   gk_string* a_string,
							   FILE* a_fout);
void		alpheiosDumpFlag(const char* a_tag,
							 const MorphEntry* a_table,
							 long a_flags,
							 FILE* a_fout);
void		alpheiosDumpFlags(const char* a_tag,
							  const MorphEntry* a_table,
							  long a_flags,
							  FILE* a_fout);
const char*	alpheiosMorphLookup(const MorphEntry* a_table, long a_flags);
const char*	alpheiosAttributeLookup(const AttributeEntry* a_table,
									const char* a_name);
bool		isEmptyForm(word_form);

/* print out info on a word */
int	alpheiosPrintWord(gk_word* gkword, PrntFlags prntflags, FILE* fout)
{
	int	nanals = totanal_of(gkword);
    SortAnals(analysis_of(gkword), nanals);

    if (prntflags & PERSEUS_FORMAT)
    {
        alpheiosDumpWord(gkword, prntflags, fout);
        return nanals;
    }

    return 0;
}

/* dump out info on a single word */
void	alpheiosDumpWord(gk_word* gkword, PrntFlags prntflags, FILE* fout)
{
	int nanals = totanal_of(gkword);
	int goodanals = GoodAnals(gkword,0);
    char curlem[MAXWORDSIZE];
    *curlem = '\0';

    /* start word */
    if (nanals > 0)
    {
        fprintf(fout, "<word>\n");
        fprintf(fout, "<form xml:lang=\"%s\">", get_xml_lang());
        xml_write_text(fout, rawword_of(gkword));
        fprintf(fout, "</form>\n");
    }

    /* for each analysis */
    gk_analysis* nxtAnalysis = analysis_of(gkword);
    gk_analysis* endAnalysis = nxtAnalysis + nanals;
    for (; nxtAnalysis != endAnalysis; ++nxtAnalysis)
    {
        if (prntflags & SHOW_FULL_INFO)
        {
            fprintf(fout, "<dump_analysis>\n");
            alpheiosDumpString("self", "  ", (gk_string*) nxtAnalysis, fout);
            if (*(nxtAnalysis->st_dictform))
            {
                fprintf(fout, "  <dictform>"); 
                xml_write_text(fout, nxtAnalysis->st_dictform); 
                fprintf(fout, "</dictform>\n");
            }
            if (*(nxtAnalysis->st_engform))
            {
                fprintf(fout, "  <engform>"); 
                xml_write_text(fout, nxtAnalysis->st_engform); 
                fprintf(fout, "</engform>\n");
            }
            alpheiosDumpString("preverb", "  ", &nxtAnalysis->gs_preverb, fout);
            alpheiosDumpString("aug1", "  ", &nxtAnalysis->gs_aug1, fout);
            alpheiosDumpString("stem", "  ", &nxtAnalysis->gs_stem, fout);
            alpheiosDumpString("suffix", "  ", &nxtAnalysis->gs_suffix, fout);
            alpheiosDumpString("end", "  ", &nxtAnalysis->gs_endstring, fout);
            if (*(nxtAnalysis->st_rawprvb))
            {
                fprintf(fout, "  <rawprvb>"); 
                xml_write_text(fout, nxtAnalysis->st_rawprvb); 
                fprintf(fout, "</rawprvb>\n");
            }
            if (*(nxtAnalysis->st_rawword))
            {
                fprintf(fout, "  <rawword>"); 
                xml_write_text(fout, nxtAnalysis->st_rawword); 
                fprintf(fout, "</rawword>\n");
            }
            if (*(nxtAnalysis->st_workword))
            {
                fprintf(fout, "  <wkword>"); 
                xml_write_text(fout, nxtAnalysis->st_workword); 
                fprintf(fout, "</wkword>\n");
            }
            if (*(nxtAnalysis->st_crasis))
            {
                fprintf(fout, "  <crasis>"); 
                xml_write_text(fout, nxtAnalysis->st_crasis); 
                fprintf(fout, "</crasis>\n");
            }
            if (*(nxtAnalysis->z))
            {
                fprintf(fout, "  <z>"); 
                xml_write_text(fout, nxtAnalysis->z); 
                fprintf(fout, "</z>\n");
            }
            fprintf(fout, "</dump_analysis>\n");
        }

        /* if there are no good analyses or this is a good one */
        /* (lemma does not contain hyphen) */
        if (!goodanals || !strchr(lemma_of(nxtAnalysis), '-'))
        {
            /* if this is a new lemma */
            if (strcmp(curlem, lemma_of(nxtAnalysis)))
            {
                /* terminate last entry */
                if (*curlem)
                    fprintf(fout, "</entry>\n");

                strcpy(curlem, lemma_of(nxtAnalysis));

                /* start new entry */
                fprintf(fout, "<entry>\n");

                /* put out info on lemma */
                fprintf(fout, "<dict>\n");
                fprintf(fout, "<hdwd xml:lang=\"%s\">", get_xml_lang());
                xml_write_text(fout, curlem);
                fprintf(fout, "</hdwd>\n");

                /* put out part of speech for first instance */
                /* as part of speech for lemma */
                /* (should we be doing this at all?) */
				const char*	pofs = alpheiosDumpPartOfSpeech(nxtAnalysis,
															fout,
															1);

                /* put out gender for noun */
                if (pofs && strcmp(pofs, "noun") == 0)
                {
                    const char *gendName = alpheiosMorphLookup(alpheiosGenderNames, gender_of(forminfo_of(nxtAnalysis)));
                    if (gendName)
                    {
                        fprintf(fout, "<gend>");
                        xml_write_text(fout, gendName);
                        fprintf(fout, "</gend>\n");
                    }
                }

                fprintf(fout, "</dict>\n");
            }

            alpheiosDumpAnalysis(nxtAnalysis, fout);
        }
		continue;
    }

    /* terminate last entry */
    if (*curlem)
        fprintf(fout, "</entry>\n");

    /* terminate word */
    if (nanals > 0)
        fprintf(fout, "</word>\n");
}

void			alpheiosDumpAnalysis(
gk_analysis*	analysis,
FILE*			fout)
{
    	/*
		Note: The lookup tables for gender and case may
		return a multi-valued string (with values separated by "/")
		so we need to iterate and produce an inflection element for
		each combination of gender and case.
	*/
    
	/* calculate term (stem + suffix) to display */
	char	stem[BUFSIZ];
	char	suffix[BUFSIZ];
	char	temp[BUFSIZ];
	int		stemlen = 0;
	int		suffixlen = 0;
    *stem = '\0';
    *suffix = '\0';

    /* build stem from preverb, aug1, stem, with colons between pieces */
	const char*	part = preverb_of(analysis);
    if (part && *part)
    {
        strncat(stem, part, BUFSIZ - 1);
        stemlen = strlen(stem);
    }
	/* aug1's containing > seem to indicate form changes already present */
	/* in other parts; those without represent a new piece */
    part = aug1_of(analysis);
    if (part && *part && !strchr(part, '>'))
    {
        if (stemlen > 0)
            strncat(stem, ":", BUFSIZ - stemlen - 1);
        strncat(stem, part, BUFSIZ - strlen(stem) - 1);
        stemlen = strlen(stem);
    }
    part = stem_of(analysis);
    if (part && *part)
    {
        if (stemlen > 0)
            strncat(stem, ":", BUFSIZ - stemlen - 1);
        strncat(stem, part, BUFSIZ - strlen(stem) - 1);
        stemlen = strlen(stem);
    }

    /* build suffix from suffix and endstring */
    part = suffix_of(analysis);
    if (part && *part)
    {
        strncat(suffix, part, BUFSIZ - 1);
        suffixlen = strlen(suffix);
    }
    part = endstring_of(analysis);
    if (part && *part)
    {
        if (suffixlen > 0)
            strncat(suffix, ":", BUFSIZ - suffixlen - 1);
        strncat(suffix, part, BUFSIZ - strlen(suffix) - 1);
        suffixlen = strlen(suffix);
    }

    /* get case(s), initialize ptrs to first case */
	word_form	wf = forminfo_of(analysis);
	const char*	caseNames = alpheiosMorphLookup(alpheiosCaseNames, case_of(wf));
	const char*	nextCase;
	const char*	endCase;
    if (caseNames)
    {
        nextCase = caseNames;
        endCase = strchr(nextCase, '/');
        if (!endCase)
            endCase = nextCase + strlen(nextCase);
    }
    else
    {
        nextCase = endCase = "";
    }

    /* for each case (using empty string if none exist) */
    while (nextCase)
    {
        /* get gender(s), initialize ptrs to first gender */
		const char*	genderNames = alpheiosMorphLookup(alpheiosGenderNames,
													  gender_of(wf));
		const char*	nextGender;
		const char*	endGender;
        if (genderNames)
        {
            nextGender = genderNames;
            endGender = strchr(nextGender, '/');
            if (!endGender)
                endGender = nextGender + strlen(nextGender);
        }
        else
        {
            nextGender = endGender = "";
        }

        /* for each gender (using empty string if none exist) */
        while (nextGender)
        {
            fprintf(fout, "<infl>\n");

            /* put out term */
            fprintf(fout, "<term xml:lang=\"%s\">", get_xml_lang());
            if (stemlen > 0)
            {
                fprintf(fout, "<stem>");
                xml_write_text(fout, stem);
                fprintf(fout, "</stem>");
            }
            if (suffixlen > 0)
            {
                fprintf(fout, "<suff>");
                xml_write_text(fout, suffix);
                fprintf(fout, "</suff>");
            }
            fprintf(fout, "</term>\n");

            /* put out part of speech */
            alpheiosDumpPartOfSpeech(analysis, fout, 0);

            /* dump case and gender (if any) and other morphological info */
            int caseLen = endCase - nextCase;
			int	genderLen = endGender - nextGender;
			if (caseLen)
            {
                strncpy(temp, nextCase, caseLen);
                temp[caseLen] = '\0';
                const char *ord = alpheiosAttributeLookup(alpheiosCaseOrder, temp);
                fprintf(fout, "<case order=\"");
                xml_write_text(fout, ord ? ord : "0");
                fprintf(fout, "\">");
                xml_write_text(fout, temp);
                fprintf(fout, "</case>\n");
            }
            if (genderLen)
            {
                strncpy(temp, nextGender, genderLen);
                temp[genderLen] = '\0';
                fprintf(fout, "<gend>");
                xml_write_text(fout, temp);
                fprintf(fout, "</gend>\n");
            }
            alpheiosDumpMorphology(wf, fout);

            /* other info: geographic region, dialect, types, etc. */
			alpheiosDumpFlags("geo",
							 alpheiosGeoNames,
							 geogregion_of(analysis),
							 fout);
			alpheiosDumpFlags("dial",
							 alpheiosDialectNames,
							 dialect_of(analysis),
							 fout);
            const char* val = NameOfStemtype(stemtype_of(analysis));
            if (val && *val)
            {
                fprintf(fout, "<stemtype>");
                xml_write_text(fout, val);
                fprintf(fout, "</stemtype>\n");
            }
            
            val = NameOfDerivtype(derivtype_of(analysis));
            if (val && *val)
            {
                fprintf(fout, "<derivtype>");
                xml_write_text(fout, val);
                fprintf(fout, "</derivtype>\n");
            }
            
            *temp = '\0';
            MorphNames(morphflags_of(analysis), temp, " ", 1);
            if (*temp)
            {
                fprintf(fout, "<morph>");
                xml_write_text(fout, temp);
                fprintf(fout, "</morph>\n");
            }

            fprintf(fout, "</infl>\n");

            /* advance to next gender */
            if (*endGender == '/')
            {
                nextGender = endGender + 1;
                endGender = strchr(nextGender, '/');
                if (!endGender)
                    endGender = nextGender + strlen(nextGender);
            }
            else
            {
                nextGender = NULL;
            }
        }

        /* advance to next case */
        if (*endCase == '/')
        {
            nextCase = endCase + 1;
            endCase = strchr(nextCase, '/');
            if (!endCase)
                endCase = nextCase + strlen(nextCase);
        }
        else
        {
            nextCase = NULL;
        }
    }
}

/* dump part of speech */
const char*		alpheiosDumpPartOfSpeech(
gk_analysis*	analysis,
FILE*			fout,
int				nopart)
{
    /* check various part of speech forms */
	const char*	pofs = NULL;
    if (Is_participle(analysis))
    {
        /* if not looking for participles, say it's a verb */
        pofs = (nopart ? "verb" : "verb participle");
    }
    else if (Is_nounform(analysis))
    {
        pofs = "noun";
    }
    else if (Is_adjform(analysis))
    {
        pofs = "adjective";
    }
    else if (Is_verbform(analysis))
    {
        pofs = "verb";
    }

    /* check stemtype and adjust part of speech */
	const char*	stemType = NameOfStemtype(stemtype_of(analysis));
    if (stemType && *stemType)
    {
        if (strstr(stemType, "pron") ||
                 !strcmp(stemType, "indef") ||
                 !strcmp(stemType, "relative") ||
                 !strcmp(stemType, "demonstr") ||
                 !strcmp(stemType, "art_adj"))
            pofs = "pronoun";
        else if (strstr(stemType, "_adj"))
            pofs = "adjective";
        else if (!strcmp(stemType, "adverb") ||
                 !strcmp(stemType, "article") ||
                 !strcmp(stemType, "particle") ||
                 !strcmp(stemType, "numeral"))
            pofs = stemType;
        else if (!strcmp(stemType, "conj"))
            pofs = "conjunction";
        else if (!strcmp(stemType, "exclam"))
            pofs = "exclamation";
        else if (!strcmp(stemType, "indecl"))
            pofs = "irregular";
        else if (!strcmp(stemType, "prep"))
            pofs = "preposition";
    }

    /* if part of speech found */
    if (pofs)
    {
        const char *ord = alpheiosAttributeLookup(alpheiosPofsOrder, pofs);
        fprintf(fout, "<pofs order=\"");
        xml_write_text(fout, ord ? ord : "0");
        fprintf(fout, "\">");
        xml_write_text(fout, pofs);
        fprintf(fout, "</pofs>\n");

        /* if noun or adjective, look for declension */
        if ((strcmp(pofs, "noun") == 0) ||
            (strcmp(pofs, "adjective") == 0))
        {
            alpheiosDumpFlag("decl",
                             alpheiosDeclNames,
                             stemtype_of(analysis) & DECL_MASK,
                             fout);
        }
    }

    return pofs;
}

/* dump morphological values (except case and gender) */
void	alpheiosDumpMorphology(word_form a_wf, FILE* a_fout)
{
    alpheiosDumpFlag("comp", alpheiosComparisonNames, degree_of(a_wf), a_fout);
    alpheiosDumpFlag("mood", alpheiosMoodNames, mood_of(a_wf), a_fout);
    alpheiosDumpFlag("num", alpheiosNumberNames, number_of(a_wf), a_fout);
    alpheiosDumpFlag("pers", alpheiosPersonNames, person_of(a_wf), a_fout);
    alpheiosDumpFlag("tense", alpheiosTenseNames, tense_of(a_wf), a_fout);
    alpheiosDumpFlag("voice", alpheiosVoiceNames, voice_of(a_wf), a_fout);
}

void		alpheiosDumpString(
const char*	a_label,
const char* a_indent,
gk_string*	a_string,
FILE*		a_fout)
{
    /* if no content, don't do anything */
    int i;
    for (i = 0; i < MORPHFLAG_BYTES; ++i)
    {
        if (a_string->gs_morphflags[i])
            break;
    }
    if ((i == MORPHFLAG_BYTES) &&
        isEmptyForm(a_string->gs_forminfo) &&
        !a_string->gs_steminfo &&
        !a_string->gs_derivtype &&
        !a_string->gs_dialect &&
        !a_string->gs_geogregion &&
        !*(a_string->st_domains) &&
        !*(a_string->gs_gkstring))
    {
        return;
    }

    fprintf(a_fout, "%s<%s>\n", a_indent, a_label);
    if (!isEmptyForm(a_string->gs_forminfo))
    {
        unsigned formval = 0;
        memcpy(&formval, &a_string->gs_forminfo, sizeof formval);
        fprintf(a_fout, "%s  <form>0%o</form>\n", a_indent, formval);
        if (a_string->gs_forminfo.f_voice)
        {
            fprintf(a_fout, "%s    <form.voice>0%o</form.voice>\n",
                    a_indent,
                    a_string->gs_forminfo.f_voice);
        }
        if (a_string->gs_forminfo.f_mood)
        {
            fprintf(a_fout, "%s    <form.mood>0%o</form.mood>\n",
                    a_indent,
                    a_string->gs_forminfo.f_mood);
        }
        if (a_string->gs_forminfo.f_tense)
        {
            fprintf(a_fout, "%s    <form.tense>0%o</form.tense>\n",
                    a_indent,
                    a_string->gs_forminfo.f_tense);
        }
        if (a_string->gs_forminfo.f_person)
        {
            fprintf(a_fout, "%s    <form.person>0%o</form.person>\n",
                    a_indent,
                    a_string->gs_forminfo.f_person);
        }
        if (a_string->gs_forminfo.f_number)
        {
            fprintf(a_fout, "%s    <form.number>0%o</form.number>\n",
                    a_indent,
                    a_string->gs_forminfo.f_number);
        }
        if (a_string->gs_forminfo.f_case)
        {
            fprintf(a_fout, "%s    <form.case>0%o</form.case>\n",
                    a_indent,
                    a_string->gs_forminfo.f_case);
        }
        if (a_string->gs_forminfo.f_degree)
        {
            fprintf(a_fout, "%s    <form.degree>0%o</form.degree>\n",
                    a_indent,
                    a_string->gs_forminfo.f_degree);
        }
        if (a_string->gs_forminfo.f_gender)
        {
            fprintf(a_fout, "%s    <form.gender>0%o</form.gender>\n",
                    a_indent,
                    a_string->gs_forminfo.f_gender);
        }
    }
    if (a_string->gs_steminfo)
    {
        const char *name = NameOfStemtype(stemtype_of(a_string));
        fprintf(a_fout, "%s  <stem>0%o ",
                a_indent,
                a_string->gs_steminfo);
        xml_write_text(a_fout, name ? name : "");
        fprintf(a_fout, "</stem>\n");
    }
    if (a_string->gs_derivtype)
    {
        fprintf(a_fout, "%s  <deriv>0%o</deriv>\n",
                a_indent,
                a_string->gs_derivtype);
    }
    if (a_string->gs_dialect)
    {
        fprintf(a_fout, "%s  <dial>0%o</dial>\n",
                a_indent,
                a_string->gs_dialect);
    }
    if (a_string->gs_geogregion)
    {
        fprintf(a_fout, "%s  <geo>0%o</geo>\n",
                a_indent,
                a_string->gs_geogregion);
    }
    if (i < MORPHFLAG_BYTES)
    {
        fprintf(a_fout, "%s  <morph>", a_indent);
        for (i = 0; i < MORPHFLAG_BYTES; ++i)
        {
            if (i > 0)
                fprintf(a_fout, ",");
            fprintf(a_fout, "%d", a_string->gs_morphflags[i]);
        }
        fprintf(a_fout, "</morph>\n");
    }
    if (*(a_string->st_domains))
    {
        fprintf(a_fout, "%s  <dom>", a_indent);
        xml_write_text(a_fout, a_string->st_domains);
        fprintf(a_fout, "</dom>\n");
    }
    if (*(a_string->gs_gkstring))
    {
        fprintf(a_fout, "%s  <str>", a_indent);
        xml_write_text(a_fout, a_string->gs_gkstring);
        fprintf(a_fout, "</str>\n");
    }
    fprintf(a_fout, "%s</%s>\n", a_indent, a_label);
}

void				alpheiosDumpFlag(
const char*			a_tag,
const MorphEntry*	a_table,
long				a_flags,
FILE*				a_fout)
{
    const char* name = alpheiosMorphLookup(a_table, a_flags);
    if (name && *name)
    {
        fprintf(a_fout, "<%s>", a_tag);
        xml_write_text(a_fout, name);
        fprintf(a_fout, "</%s>\n", a_tag);
    }
}

void				alpheiosDumpFlags(
const char*			a_tag,
const MorphEntry*	a_table,
long				a_flags,
FILE*				a_fout)
{
    if (!a_flags || !a_table)
        return;

	char	temp[BUFSIZ];
	*temp = '\0';

	const MorphEntry*	nextEntry;
    for (nextEntry = a_table; nextEntry->d_flags != 0; ++nextEntry)
    {
        /* if this entry is contained in flags */
        if ((nextEntry->d_flags & a_flags) == nextEntry->d_flags)
        {
            /* mask out used flags and add to output */
            a_flags &= ~(nextEntry->d_flags);
            if (*temp)
                strcat(temp, " ");
            strncat(temp, nextEntry->d_name, BUFSIZ - strlen(temp) - 1);
        }
    }

    if (*temp)
    {
        fprintf(a_fout, "<%s>", a_tag);
        xml_write_text(a_fout, temp);
        fprintf(a_fout, "</%s>\n", a_tag);
    }
}

const char*	alpheiosMorphLookup(
const MorphEntry*	a_table,
long				a_flags)
{
    if (!a_flags || !a_table)
        return NULL;

	const MorphEntry*	nextEntry;
    for (nextEntry = a_table; nextEntry->d_flags != 0; ++nextEntry)
    {
        if ((nextEntry->d_flags & a_flags) == a_flags)
            return nextEntry->d_name;
    }

    return NULL;
}

const char*				alpheiosAttributeLookup(
const AttributeEntry*	a_table,
const char*				a_name)
{
    if (!a_name || !a_table)
        return NULL;

	const AttributeEntry*	nextEntry;
    for (nextEntry = a_table; nextEntry->d_name != NULL; ++nextEntry)
    {
        if (strcmp(nextEntry->d_name, a_name) == 0)
            break;
    }

    return nextEntry->d_value;
}

bool	isEmptyForm(word_form a_wf)
{
    return !a_wf.f_voice &&
           !a_wf.f_mood &&
           !a_wf.f_tense &&
           !a_wf.f_person &&
           !a_wf.f_number &&
           !a_wf.f_case &&
           !a_wf.f_degree &&
           !a_wf.f_gender;
}
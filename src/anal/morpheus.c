#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <gkstring.h>

#include "../greeklib/xstrings.proto.h"
#include "../greeklib/stripbreath.proto.h"
#include "../greeklib/addbreath.proto.h"
#include "../morphlib/morphpath.proto.h"
#include "../morphlib/setlang.proto.h"
#include "../morphlib/trimwhite.proto.h"
#include "../morphlib/gkstring.proto.h"
#include "prntalph.h"
#include "checkstring.proto.h"

#define ARGS		"ILcixSVpn" 

int quickflag = 0;

void trimdigit(char *s)
{
    char *p = s;
    while (*s) s++;
    s--;
    while (isdigit((unsigned char)*s) && s > p) *s-- = 0;
}

void print_safe_comment(const char *s)
{
    fputs("<!-- ", stdout);
    for (; *s; s++)
    {
        if (s[0] == '-' && s[1] == '-')
        {
            fputs("- ", stdout);
        }
        else
        {
            putchar((unsigned char)*s);
        }
    }
    fputs(" -->\n", stdout);
}

int main(int argc, char** argv)
{
    char		line[BUFSIZ*4];
    char		word[BUFSIZ*4];
    PrntFlags	flags = (PERSEUS_FORMAT|STRICT_CASE);
    int			c, errflg = 0;

    /* process arguments */
    while (!errflg && (c = getopt(argc, argv, ARGS)) != -1)
    {
        switch (c)
        {
          case 'c': flags |= CHECK_PREVERB; break;
          case 'I': set_lang(ITALIAN); break;
          case 'L': set_lang(LATIN); break;
          case 'i': flags |= SHOW_FULL_INFO; break;
          case 'x': flags |= LEXICON_OUTPUT; break;
          case 'V': flags |= VERBS_ONLY; break;
          case 'S': flags &= ~(STRICT_CASE); break;
          case 'n': flags |= IGNORE_ACCENTS; break;
          case 'p': flags |= PARSE_FORMAT; break;
          default:  errflg++; break;
        }
    }

    int had_args = (optind < argc);
    int started = 0;
    int nwords = 0;
    int nunknown = 0;

    while (1)
    {
        if (optind < argc)
        {
            Xstrncpy(word, argv[optind++], sizeof(word));
            trimdigit(word);
        }
        else
        {
            if (had_args) break;

            if (!fgets(line, sizeof(line), stdin)) break;
            
            trimwhite(line);
            if (isspace((unsigned char)line[0]) || !line[0]) continue;
            
            if (line[0] == '#')
            {
                if (!started) { printf("<words>\n"); started = 1; }
                print_safe_comment(line + 1);
                continue;
            }

            trimdigit(line);
            char *p = line;
            while (*p && !isspace((unsigned char)*p)) p++;
            if (p == line) continue;
            *p = 0;
            
            Xstrncpy(word, line, sizeof(word));
        }

        if (!started) { printf("<words>\n"); started = 1; }

        gk_word *gkword = AnalyzeString(word, flags);
        int rval = gkword ? totanal_of(gkword) : 0;
        
        if (cur_lang() != LATIN && cur_lang() != ITALIAN && !rval && (flags & IGNORE_ACCENTS))
        {
            char tmpform[BUFSIZ];
            if (gkword) FreeGkword(gkword);

            Xstrncpy(tmpform, word, sizeof(tmpform));
            stripbreath(tmpform);
            addbreath(tmpform, ')');
            gkword = AnalyzeString(tmpform, flags);
            rval = gkword ? totanal_of(gkword) : 0;

            if (!rval)
            {
                if (gkword) FreeGkword(gkword);
                stripbreath(tmpform);
                addbreath(tmpform, '(');
                gkword = AnalyzeString(tmpform, flags);
                rval = gkword ? totanal_of(gkword) : 0;
            }
        }

        if (rval > 0)
        {
            alpheiosPrintWord(gkword, flags, stdout);
        }
        else
        {
            printf("<unknown xml:lang=\"%s\">", get_xml_lang());
            xml_write_text(stdout, word);
            printf("</unknown>\n");
            ++nunknown;
        }

        if (gkword) FreeGkword(gkword);
        ++nwords;
    }

    if (!started) {
        printf("<error>No words provided</error>\n");
        return 2;
    } else {
        printf("</words>\n");
        fprintf(stderr, "%d word%s analyzed, %d unknown\n",
            nwords, nwords == 1 ? "" : "s", nunknown);
    }

    return 0;
}
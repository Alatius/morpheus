#ifndef PRNTALPH_H
#define PRNTALPH_H

#include <libfiles.h>
#include <gkstring.h>

int alpheiosPrintWord(gk_word *gkword, PrntFlags prntflags, FILE *fout);
void xml_write_text(FILE *f, const char *s);
const char *get_xml_lang(void);

#endif /* PRNTALPH_H */
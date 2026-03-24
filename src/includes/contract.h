#ifndef CONTRACT_H
#define CONTRACT_H

/*
 * Greg Crane 
 * June 1987
 */

#include <gkstring.h>
#define MAXCONTRACTS 10
#define MAXSUBSTRING 10
Dialect AndDialect(Dialect, Dialect);
#include "libfiles.h"


gk_string *poss_contracts(gk_string *, Dialect);
gk_string *load_euph_tab(char *, int *, int);
char *is_substring(char *, char *);

static gk_string Blnk;

#endif /* CONTRACT_H */

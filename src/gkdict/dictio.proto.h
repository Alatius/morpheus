#ifndef DICTIO_PROTO_H
#define DICTIO_PROTO_H


/* dictio.c */
endtags *init_dict(char *fname, int *ntags);
int chckirrverb(char *irregstr, char *lemmas);
int chckindecl(char *indeclstr, char *lemmas);
int chckderiv(char *derivstr, char *derivkeys);
int chckstem(char *stemstr, char *stemkeys, int is_nom);
FILE *getlemmstart(char *lemma, char *lemmfile, long *lemmoff);
int lemma_exists(char *lemma);

#endif /* DICTIO_PROTO_H */

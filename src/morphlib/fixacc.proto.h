#ifndef FIXACC_PROTO_H
#define FIXACC_PROTO_H


/* fixacc.c */
int putsimpleacc(char *);
int FixRecAcc(gk_word *, MorphFlags *, char *);
int FixPersAcc(gk_string *, MorphFlags *, gk_string *, char *, char *, word_form, int);

#endif /* FIXACC_PROTO_H */

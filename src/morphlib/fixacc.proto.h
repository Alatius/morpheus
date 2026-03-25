#ifndef FIXACC_PROTO_H
#define FIXACC_PROTO_H


/* fixacc.c */
void putsimpleacc(char *);
void FixRecAcc(word_form, gk_string *, MorphFlags *, char *);
void FixPersAcc(gk_string *, MorphFlags *, gk_string *, char *, char *, word_form, int);
void FixPersAcc2(gk_string *, MorphFlags *, gk_string *, char *, char *, word_form, int);

#endif /* FIXACC_PROTO_H */

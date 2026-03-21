#ifndef PREVERB2_PROTO_H
#define PREVERB2_PROTO_H


/* preverb2.c */
int CombPbStem(char *, char *, Dialect, MorphFlags *);
int CombPbStemL(char *, char *, Dialect, MorphFlags *);
int CombPbStemG(char *, char *, Dialect, MorphFlags *);
int is_preverb(char *, char *, gk_string *);
int exp_preverb(char *, char *, gk_string *);

#endif /* PREVERB2_PROTO_H */

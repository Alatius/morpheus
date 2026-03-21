#ifndef RETRENDS_PROTO_H
#define RETRENDS_PROTO_H


/* retrends.c */
gk_string *chckendings(char *, char *, char *, char *, Dialect, int *);
int CompatKeys(char *, char *, gk_string *);
int EndingOk(char *, gk_string *, gk_string *, int);
int WantGkEnd(gk_string *, gk_string *, bool, int);
int RightMorphflags(gk_string *, gk_string *);
void setwendstr(char *, char *);
int endstrcmp(char *, char *);
int noaccstrcmp(char *, char *);
int has_quantacc(char *);
void stripquantacc(char *);

#endif /* RETRENDS_PROTO_H */

#ifndef CHECKSTEM_PROTO_H
#define CHECKSTEM_PROTO_H


/* checkstem.c */
int checkstem(char *, char *, gk_string *[], char *[], int);
int stemexists(char *, char *, char *, int);
int setstemvars(char *, char *, char *, char *, char *);
char *parsefield(char *, char *, int, int);

#endif /* CHECKSTEM_PROTO_H */

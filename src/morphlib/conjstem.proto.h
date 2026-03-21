#ifndef CONJSTEM_PROTO_H
#define CONJSTEM_PROTO_H


/* conjstem.c */
void fixcontr(char *, char *);
void makeperf(char *);
void fixperf(char *);
void conjstem(char *, char *);
void conjoin(char *, char *);
int do_sigma(char *, char *);
int do_theta(char *);
int do_mu(char *);
int do_tau(char *);

#endif /* CONJSTEM_PROTO_H */

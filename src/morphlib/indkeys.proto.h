#ifndef INDKEYS_PROTO_H
#define INDKEYS_PROTO_H


/* indkeys.c */
void index_list(char *, char *, int);
#ifdef DECALPHA
void prockeyline(char *, int, int, FILE *);
#else
void prockeyline(char *, int, long, FILE *);
#endif

#endif /* INDKEYS_PROTO_H */

#ifndef MORPHLIB_RETRENTRY_PROTO_H
#define MORPHLIB_RETRENTRY_PROTO_H


/* retrentry.c */
endtags *init_preind(char *, int *);
long ChckPreIndex(endtags *, char *, int, int, int (*scmp)(const char *, const char *));
int ChckFullIndex(char *, char *, char *, long, int (*scmp)(const char *, const char *, size_t));

#endif /* MORPHLIB_RETRENTRY_PROTO_H */

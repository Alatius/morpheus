#ifndef MORPHLIB_MORPHSTRCMP_PROTO_H
#define MORPHLIB_MORPHSTRCMP_PROTO_H


/* morphstrcmp.c */
int morphstrcmp(char *, char *);
int morphstrncmp(char *, char *, size_t);
int dictstrcmp(char *, char *);
int dictstrncmp(char *, char *, size_t);
int init_comptab(void);

#endif /* MORPHLIB_MORPHSTRCMP_PROTO_H */

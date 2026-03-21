#ifndef MORPHLIB_MORPHSTRCMP_PROTO_H
#define MORPHLIB_MORPHSTRCMP_PROTO_H


/* morphstrcmp.c */
int morphstrcmp(char *, char *);
int betastrcmp(char *, char *);
int morphstrncmp(char *, char *, size_t);
int dictstrcmp(char *, char *);
int dictstrncmp(char *, char *, size_t);
void init_comptab(void);
void init_betatab(void);

#endif /* MORPHLIB_MORPHSTRCMP_PROTO_H */

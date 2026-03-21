#ifndef ENDINDEX_PROTO_H
#define ENDINDEX_PROTO_H


/* endindex.c */
int chcknend(char *, char *);
int chckvend(char *, char *);
int chckvstem(char *, char *);
int chckdvend(char *, char *);
endind *init_endind(char *, endind *);
int checkendind(endind *, char *, char *, int (*scmp )());

#endif /* ENDINDEX_PROTO_H */

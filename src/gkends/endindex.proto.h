#ifndef ENDINDEX_PROTO_H
#define ENDINDEX_PROTO_H


/* endindex.c */
int chcknend(char *, char *);
int chckvend(char *, char *);
int chckvstem(char *, char *);
int chckdvend(char *, char *);
int chckcmpvb(char *, char *);
int chckend(char *);
endind *init_endind(char *, endind *);
int checkendind(endind *, char *, char *, int (*scmp)());

int chckdictent(char *, char *);

#endif /* ENDINDEX_PROTO_H */

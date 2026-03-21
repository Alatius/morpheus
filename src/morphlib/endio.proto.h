#ifndef ENDIO_PROTO_H
#define ENDIO_PROTO_H


/* endio.c */
int WriteEnding(FILE *, gk_string *, int);
int ReadEnding(FILE *, gk_string *, int);
int set_endheader(FILE *, int);
int get_endheader(FILE *, int *);

#endif /* ENDIO_PROTO_H */

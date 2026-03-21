#ifndef VAXWORDS_PROTO_H
#define VAXWORDS_PROTO_H


/* vaxwords.c */
void get_int32(int32 *, FILE *);
void put_int32(int32 *, FILE *);
void get_double(double *, int, FILE *);
void put_double(double *, int, FILE *);
void get_short(unsigned short *, FILE *);
void put_short(short *, FILE *);
int vax_fread(char *, size_t, int, FILE *);
int vax_fwrite(char *, size_t, int, FILE *);

#endif /* VAXWORDS_PROTO_H */

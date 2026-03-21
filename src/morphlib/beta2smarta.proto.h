#ifndef BETA2SMARTA_PROTO_H
#define BETA2SMARTA_PROTO_H


/* beta2smarta.c */
void beta2smarta(char *, char *);
void beta2smk(char *, char *);
void set_greek(void);
void set_roman(void);
void beta2mac(char *, char *, int);
int accnum(int);
char *romanfont(char *);
char *greekfont(char *);
int smk_char_xlit(int, char *, int);

#endif /* BETA2SMARTA_PROTO_H */

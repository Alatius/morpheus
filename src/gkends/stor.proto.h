#ifndef STOR_PROTO_H
#define STOR_PROTO_H


/* stor.c */
int InitGstrMem(void);
int AddNewGstr(gk_string *);
int PrntNewGstrings(FILE *, int);
int LPrntGstr(gk_string *, FILE *);
int new_parad(gk_string *, gk_string *);
int hyphtodiaer(char *);

#endif /* STOR_PROTO_H */

#ifndef STOR_PROTO_H
#define STOR_PROTO_H


/* stor.c */
int InitGstrMem(void);
void AddNewGstr(gk_string *);
void PrntNewGstrings(FILE *, int);
void LPrntGstr(gk_string *, FILE *);
int new_parad(gk_string *, gk_string *);
void hyphtodiaer(char *);

#endif /* STOR_PROTO_H */

#ifndef PROPNAME_PROTO_H
#define PROPNAME_PROTO_H


/* propname.c */
int main(void);
void trimdigit(char *);
int checkpropname(char *, FILE *);
int tryxlits(FILE *, char *, char *, char *, char *, char *);
void Xliterate(char *, char *);
int checkposs(FILE *, char *, char *, char *, char *, char *);
int stem_can_work(char *, char *, char *, char *, char *, char *);
void reliterate(char *);
int is_propname(char *);
int AlphStrcmp(char *, char *);

#endif /* PROPNAME_PROTO_H */

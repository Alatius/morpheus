#ifndef GENSYNFORM_PROTO_H
#define GENSYNFORM_PROTO_H

/* gensynform.c */
int GenSynForms(FILE *, FILE *);
int is_pass_stem(Stemtype);
int is_mp_stem(Stemtype);
int is_exception(char *, char *);

#endif /* GENSYNFORM_PROTO_H */

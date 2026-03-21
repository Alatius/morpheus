#ifndef BETA2RTF_PROTO_H
#define BETA2RTF_PROTO_H


/* beta2rtf.c */
int _main(int, char *[]);
int conv_defline(char *, FILE *);
int check_deflev(char *, char *, int);
int has_pref(char *, char *);
int is_greek(char *);

#endif /* BETA2RTF_PROTO_H */

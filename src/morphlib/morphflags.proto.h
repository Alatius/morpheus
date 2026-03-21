#ifndef MORPHFLAGS_PROTO_H
#define MORPHFLAGS_PROTO_H


/* morphflags.c */
void add_morphflags(gk_string *, MorphFlags *);
void set_morphflags(gk_string *, MorphFlags *);
void set_gwmorphflags(gk_word *, MorphFlags *);
void zap_morphflags(gk_string *, MorphFlags *);
int has_morphflags(gk_string *, MorphFlags *);
int no_morphflags(gk_string *);
void add_morphflag(MorphFlags *, int);
int overlap_morphflags(MorphFlags *, MorphFlags *);
int has_morphflag(MorphFlags *, int);
void zap_morphflag(MorphFlags *, int);
void set_morphflag(MorphFlags *, int);
int no_morphflag(MorphFlags *);
void mflag_num_to_bits(int, int *, int *);
int mflag_bit_to_num(int, int);
void Dump_morphflag(MorphFlags *);
int is_pretty_morphflag(long);
void init_ugly_tab(void);
int is_prvb_morphflag(long);
void init_prvb_tab(void);
void xfer_prvbflags(MorphFlags *, MorphFlags *);
void MorphNames(MorphFlags *, char *, char *, int);

#endif /* MORPHFLAGS_PROTO_H */

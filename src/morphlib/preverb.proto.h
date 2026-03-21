#ifndef PREVERB_PROTO_H
#define PREVERB_PROTO_H


/* preverb.c */
bool checkprevb(char *, char *, bool *);
bool prvbcmp(char *, char *, bool *);
void getrest(char *, char *, char *, char *);
void rstprevb(char *, char *, gk_string *);
int First_K_aspirate(char *);
void shift_su_to_cu(char *);
void shift_eis_to_es(char *);
void shift_pros_to_poti(char *);
void shift_pros_to_proti(char *);
void shift_upo_to_upai(char *);
void shift_uper_to_upeir(char *);
void shift_para_to_parai(char *);
void shift_meta_to_peda(char *);
void shift_en_to_eni(char *);
void set_odd_prvb(MorphFlags *, char *);

#endif /* PREVERB_PROTO_H */

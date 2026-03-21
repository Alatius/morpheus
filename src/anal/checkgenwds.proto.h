#ifndef CHECKGENWDS_PROTO_H
#define CHECKGENWDS_PROTO_H


/* checkgenwds.c */
int CheckGenWords(gk_word *, gk_word *);
int AddAnalysis(gk_word *, gk_word *);
void merge_anal_dialects(gk_analysis *, gk_analysis *);
int equiv_anal(gk_analysis *, gk_analysis *);

#endif /* CHECKGENWDS_PROTO_H */

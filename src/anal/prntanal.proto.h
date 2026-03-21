#ifndef PRNTANAL_PROTO_H
#define PRNTANAL_PROTO_H


/* prntanal.c */
void PrntAnalyses(gk_word *, PrntFlags, FILE *);
char *anal_buf(void);
void PrntOneAnalysis(gk_analysis *, PrntFlags, FILE *);
void near_miss(gk_string *, char *, int);
void odd_morpheme(gk_analysis *, gk_string *, char *, char *, int);
void dump_all_anals(gk_word *, PrntFlags, FILE *);
void SortAnals(gk_analysis *, int);
void DumpLemmaInfo(gk_word *, PrntFlags , FILE *);
int CompAnals(const void*, const void*);
void DumpOneAnalysis(gk_word *, PrntFlags, gk_analysis *, FILE *, int);
void DumpEndingIndex(gk_word *, PrntFlags, gk_analysis *, FILE *, int);
void DumpPerseusAnalysis(gk_word *, PrntFlags, gk_analysis *, FILE *, int);
void DumpGstr(char *, gk_string *, FILE *, int);
void DumpDbGkString(gk_string *, FILE *);

#endif /* PRNTANAL_PROTO_H */

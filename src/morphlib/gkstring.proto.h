#ifndef GKSTRING_PROTO_H
#define GKSTRING_PROTO_H


/* gkstring.c */
gk_string *CreatGkString(int);
int FreeGkString(gk_string *);
gk_analysis *CreatGkAnal(int);
int FreeGkAnal(gk_analysis *);
gk_word *CreatGkword(int);
int ClearGkstring(gk_string *);
int FreeGkword(gk_word *);
int CpGkAnal(gk_word *, gk_word *);
int CompGkString(const void *, const void *);
int CompGkForms(const void *, const void *);
int low_bit_of(int);
int CompByDictStr(const void *, const void *);
int RevCompByStr(gk_string *, gk_string *);
int PrntGkStrings(gk_string *, FILE *);
int PrntGkParadigm(gk_string *, FILE *);
int PrntGkStr(gk_string *, FILE *);
int PrntGkFlags(gk_string *, FILE *);
int PrntDomains(char *, FILE *);
int PrntMorphFlags(MorphFlags *, FILE *);
int PrntVerbInfo(word_form, FILE *);
int PrntParadigmInfo(word_form, FILE *);
int AddParadigmInfo(char *, word_form);
int AddPersNumInfo(char *, word_form);
int PrntPersNumInfo(word_form, FILE *);
int PrntAdjInfo(word_form, FILE *);
int AddAdjInfo(char *, word_form);
int PrntStemtype(Stemtype, FILE *);
int PrntDialect(Dialect, FILE *);
int AddDialect(Dialect, char *);
Dialect AndDialect(Dialect, Dialect);
int xInsertGstr(gk_string *, gk_string *, int, int (*compare )(), int);
int GetTableLine(char *, int, FILE *);
int eq_forminfo(word_form, word_form);
int SprintGkFlags(gk_string *, char *, char *, int);
int DbaseFormat(gk_string *, char *, char *, int);

#endif /* GKSTRING_PROTO_H */

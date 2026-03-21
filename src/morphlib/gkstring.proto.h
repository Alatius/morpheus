#ifndef GKSTRING_PROTO_H
#define GKSTRING_PROTO_H


/* gkstring.c */
gk_string *CreatGkString(int);
void FreeGkString(gk_string *);
gk_analysis *CreatGkAnal(int);
void FreeGkAnal(gk_analysis *);
gk_word *CreatGkword(int);
void ClearGkstring(gk_string *);
void FreeGkword(gk_word *);
void CpGkAnal(gk_word *, gk_word *);
int CompGkString(const void *, const void *);
int CompGkForms(const void *, const void *);
int low_bit_of(int);
int CompByDictStr(const void *, const void *);
int RevCompByStr(gk_string *, gk_string *);
void PrntGkStrings(gk_string *, FILE *);
void PrntGkParadigm(gk_string *, FILE *);
void PrntGkStr(gk_string *, FILE *);
void PrntGkFlags(gk_string *, FILE *);
void PrntDomains(char *, FILE *);
void PrntMorphFlags(MorphFlags *, FILE *);
void PrntVerbInfo(word_form, FILE *);
void PrntParadigmInfo(word_form, FILE *);
void AddParadigmInfo(char *, word_form, char *);
void AddPersNumInfo(char *, word_form, char *);
void PrntPersNumInfo(word_form, FILE *);
void PrntAdjInfo(word_form, FILE *);
void AddAdjInfo(char *, word_form, char *);
void PrntStemtype(Stemtype, FILE *);
void PrntDialect(Dialect, FILE *);
void AddDialect(Dialect, char *, char *);
Dialect AndDialect(Dialect, Dialect);
int xInsertGstr(gk_string *, gk_string *, int, int (*compare )(), int);
int GetTableLine(char *, int, FILE *);
int eq_forminfo(word_form, word_form);
void SprintGkFlags(gk_string *, char *, char *, int);
void DbaseFormat(gk_string *, char *, char *, int);

#endif /* GKSTRING_PROTO_H */

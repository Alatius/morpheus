#ifndef GENWD_PROTO_H
#define GENWD_PROTO_H

/* genwd.c */
int NextDictLine(FILE *, char *, char *, char *);
int BuildAWord(gk_word *, gk_string *, gk_word *);
int BuildANoun(gk_word *, gk_string *, gk_word *);
int BuildAVerb(gk_word *, gk_string *, gk_word *);

#endif /* GENWD_PROTO_H */

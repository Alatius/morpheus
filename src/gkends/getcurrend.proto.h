#ifndef GETCURREND_PROTO_H
#define GETCURREND_PROTO_H


/* getcurrend.c */
gk_string *GetCurrentEndList(gk_string *, int *);
gk_string *CheckEndCache(gk_string *);
void cacheconsistent(void);
void InsertEndCache(gk_string *);

#endif /* GETCURREND_PROTO_H */

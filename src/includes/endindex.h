#ifndef ENDINDEX_H
#define ENDINDEX_H

typedef struct {
	char *ebuf;
	char **eptr;
	int nelems;
} endind;

#define endbuffer_of( X ) (X->ebuf)
#define endeptr_of( X ) (X->eptr)
#define endlen_of(X) (X->nelems)

endind *init_endind(char *, endind *);

#endif /* ENDINDEX_H */

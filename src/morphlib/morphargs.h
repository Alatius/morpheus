#ifndef MORPHARGS_H
#define MORPHARGS_H

typedef long Morph_flags;

typedef struct {
	char	morph_key[MAXWORDSIZE];
	Morph_flags	morph_flags;
	void 	(*add_val)();
} Morph_args;

#endif /* MORPHARGS_H */

#ifndef SMK_H
#define SMK_H

#define SMK_ALPHA(X) ((X)=='a'||(X)=='A'||((unsigned char)(X)>=0213&&(unsigned char)(X)<=0225))
#define SMK_AISUB(X) ((X)==046||((unsigned char)(X)>=0226&&(unsigned char)(X)<=0240))
#define SMK_EPSILON(X) ((X)=='e'||(X)=='E'||((unsigned char)(X)>=0241&&(unsigned char)(X)<=0250))
#define SMK_ETA(X) ((X)=='h'||(X)=='H'||((unsigned char)(X)>=0256&&(unsigned char)(X)<=0270))
#define SMK_EISUB(X) ((unsigned char)(X)==0372||((unsigned char)(X)>=0271&&(unsigned char)(X)<=0303))
#define SMK_IOTA(X) ((X)=='i'||(X)=='I'||((unsigned char)(X)>=0333&&(unsigned char)(X)<=0345))
#define SMK_OMICRON(X) ((X)=='o'||(X)=='O'||((unsigned char)(X)>=0361&&(unsigned char)(X)<=0371))
#define SMK_UPSILON(X) ((X)=='u'||(X)=='U'||((unsigned char)(X)>=0346&&(unsigned char)(X)<=0360))
#define SMK_WMEGA(X) ((X)=='v'||(X)=='V'||((unsigned char)(X)>=0305&&(unsigned char)(X)<=0317))
#define SMK_WISUB(X) ((unsigned char)(X)==0304 || ((unsigned char)(X)>=0320&&(unsigned char)(X)<=0332))
#define SMK_IDIAER(X)  ((unsigned char)(X)==0363 || (unsigned char)(X)==0375 || (unsigned char)(X)==0376)
#define SMK_UDIAER(X)  ((X)==043 || (X)==0100 || (unsigned char)(X)==0347)
#define SMK_DIAER(X) (SMK_UDIAER(X) || SMK_IDIAER(X))

#define SMK_VOWEL(X) (SMK_ALPHA(X)  || \
	SMK_AISUB(X) || \
	SMK_EPSILON(X) || \
	SMK_ETA(X) || \
	SMK_EISUB(X) || \
	SMK_IOTA(X) || \
	SMK_OMICRON(X) || \
	SMK_UPSILON(X) || \
	SMK_WMEGA(X) || \
	SMK_WISUB(X) || \
	SMK_IDIAER(X) || \
	SMK_UDIAER(X) )

#define SYNIZESIS 'X'
#define SMARTA_LONGMARK '^'
#define SMARTA_SHORTMARK 0255
#define SMK_LONGMARK '*'
#define SMK_SHORTMARK 0255
#define SMK_DAGGER '~'

#endif /* SMK_H */

#ifndef Y_TAB_H
#define Y_TAB_H


typedef union  {
        char *string;
} YYSTYPE;
extern YYSTYPE yylval;
# define ARTICLE 257
# define NOUN 258
# define ADJECTIVE 259
# define PREP 260

#endif /* Y_TAB_H */

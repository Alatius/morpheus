#ifndef MORPHPATH_PROTO_H
#define MORPHPATH_PROTO_H


/* morphpath.c */
FILE *MorphFopen(char *, char *);
int NumFilesOpened(void);
int MorphPathName(char *, char *);
int SysFolderFile(char *, char *);

#endif /* MORPHPATH_PROTO_H */

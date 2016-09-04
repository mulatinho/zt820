#ifndef _MLT_STRLIB_H_
#define _MLT_STRLIB_H_ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
	_STR_INIT,
	_STR_END,
	_STR_MATCH
} str_match;

#define DATASTR_MAX 4096

/* Function Prototypes */

char *mlt_strkey(char *buffer, int who, char del);
const char *mlt_substr(char *buffer, int left, int right);
const char **mlt_split(char *buffer, char delim);
void mlt_strupper(char *buffer);
void mlt_strcat(char *, char *);

int mlt_strnchk(char *buffer, int length);
int mlt_strndel(char *buffer, char delimiter);
int mlt_strget(char *buffer, char *in, char delim, str_match match);

char mlt_strtonum(char *, char *);
int mlt_numtoint(char *);

void mlt_strleft(char *, char *, char);

#endif

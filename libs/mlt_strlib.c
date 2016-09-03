/*
MLT_STRLIB.C - A LIB TO MANIPULATE TEXTS, STRINGS AND CHARACTERS
:. Copyright (C) 2014  Alexandre Mulatinho

This library is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
*/

#include "mlt_strlib.h"

/*
 * func: mlt_strkey - get word
 */
char *mlt_strkey(char *buffer, int who, char del)
{
	char *str = (char*)malloc(strlen(buffer));
	int i, f=0, x=0, loop=0;
	char ch;

	for (i=0;i<strlen(buffer);i++) {
		ch = *(buffer+i);

		if (ch==del && !i && !who) {
			free(str);
			return NULL;
		}

		if (ch==del) {
			loop++;
			continue;
		}	

		if (loop==who) {
			*(str+x) = *(buffer+i); 
			x++;
		}
	}

	/*if (!who)
		*(str + (x-1)) = '\0';
	else */
		*(str + x) = '\0';

	str=(char*)realloc(str,x);
	return (char*)str;
}

void mlt_strupper(char *buffer)
{
	while (*buffer != '\0') {
		if ((*buffer >= 'a') && (*buffer <= 'z'))
		       *buffer -= 32;
		
		buffer++;
	}
}
/* check length and copy buffer to inbuffer if exists the two strings */
void mlt_strcat(char *inbuffer, char *buffer)
{
	int len = 0, nlen = 0;

	if (!inbuffer || !buffer)
		return;

	nlen = strlen(inbuffer);
	len  = strlen(buffer) + nlen;

	if (len >= DATASTR_MAX)
		return;

	inbuffer[--nlen] = ' ';

	//for (; nlen < len; inbuffer[nlen++] = buffer[]);

	

}

/* check if the buffer has at least n items */
int mlt_strnchk(char *buffer, int n)
{
	if (!buffer)
		return 0;
	else if (strlen(buffer) > n)
		return 0;

	return 1;
}

/* returns the x position of the first delimiter character */
int mlt_strndel(char *buffer, char delimiter)
{
	int j = 0, n = 0;

	if (!buffer)
		return j;

	n = strlen(buffer);
	for (; j < n; j++) {
		if (buffer[j] == delimiter) {
			break;
		}
	}

	return j;
}

void mlt_substr(char *buffer, char *in, int left, int right)
{
	char result[DATASTR_MAX];
	int j = 0, n = 0, len = 0, flag = 0;

	if (!buffer)
		return;
	else if (!in)
		flag++;

	len = strlen(buffer);

	if (!left || !right || left > len || right > len)
		return;

	left--;
	right--;

	for (; j < len; j++) {
		if (j >= left && j <= right) {
			result[n++] = buffer[j];
		}
	}

	if (flag)
		fprintf(stdout, "%s", result);
//	else
//		in = 
}

/* char buffer[LENGTH] = "Message to my love: dear love, i want hang out with you";
 * mlt_strget(buffer, in, ':', _STR_INIT); 
 */
int mlt_strget(char *buffer, char *in, char delim, str_match match)
{
	int i, j, ndel, ntotal;

	if (!buffer)
		return 1;

	while (!in) 
		in = (char*)malloc(strlen(buffer));

	i = 0; j = 0; ndel = 0; ntotal = 0;

	ntotal = mlt_strndel(buffer, delim);

	for (j = 0; j < strlen(buffer); j++) {
		if (buffer[j] == delim)
			ndel++;

		if (match == _STR_INIT) {
			if (ndel)
				break;

			in[i++] = buffer[j];
		}

		if (match == _STR_END) {
			if (ndel == ntotal) {
				in[i++] = buffer[j];
			}
		}
	}
	
	in[i] = '\0';
	return 0;
}

#ifdef DEBUG
int main(void)
{
	char *buffer = "Buy me those itens, fish, milk and: soda";
	char buffertwo[] = "Here an execution of B means that portion of the execution of the program that would	correspond to the lifetime of an object with scalar type and automatic storage duration	associated with B.";
	char *p;

	fprintf(stdout, "buffer: %s\n\n", buffer);

	p = mlt_strkey(buffer, 1, ',');
	if (p) {
		fprintf(stdout, "-----\n");
		fprintf(stdout, "  func: p = mlt_strkey(buffer, 1, ',');\n");
		fprintf(stdout, "result: mlt_strkey is '%s'\nstr has '%lu' bytes\n", 
				p, strlen(p));
	}

	fprintf(stdout, "-----\n");
	fprintf(stdout, "  func: mlt_substr(buffer, NULL, 4, 12);\nresult: ");
	mlt_substr(buffer, NULL, 4, 12);

	fprintf(stdout, "\n-----\n");
	fprintf(stdout, "  func: mlt_substr(buffer, NULL, 22, 29);\nresult: ");
	mlt_substr(buffer, NULL, 22, 29);

	mlt_strget(buffer, p, ',', _STR_INIT);
	if (p) {
		fprintf(stdout, "\n-----\n");
		fprintf(stdout, "  func: mlt_strget(buffer, p, ',', _STR_INIT);\n");
		fprintf(stdout, "result: mlt_strget is '%s'\nstr has '%lu' bytes\n", 
				buffer, p, strlen(p));
	}

	mlt_strget(buffer, p, ':', _STR_END);
	if (p) {
		fprintf(stdout, "-----\n");
		fprintf(stdout, "  func: mlt_strget(buffer, p, ',', _STR_END);\n");
		fprintf(stdout, "result: mlt_strget is '%s'\nstr has '%lu' bytes\n", 
			p, strlen(p));
	}

	fprintf(stdout, "-----\n");
	fprintf(stdout, "  func: mlt_strndel(buffer, ',')\n");
	fprintf(stdout, "result: delimiter found in '%d' bytes\n", 
		mlt_strndel(buffer, ','));

	mlt_strupper(&buffertwo);
	fprintf(stdout, "\n\n-> %s\n", buffertwo); 
	
	return 0;
}
#endif

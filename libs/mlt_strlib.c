/*
MLT_STRLIB.C - A LIB TO MANIPULATE TEXTS, STRINGS AND CHARACTERS
:. Copyright (C) 2014  Alexandre Mulatinho

This library is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
*/

#include "mlt_strlib.h"

char *mlt_strkey(char *buffer, int who, char del)
{
	char *str = (char*)malloc(strlen(buffer));
	int i, x=0, loop=0;
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

	*(str + x) = '\0';

	str=(char*)realloc(str,x);
	return (char*)str;
}

int mlt_count(char *buffer, char ch)
{
	int result = 0;
	while (*buffer != '\0') {
		if (*buffer == ch)
			result++;
		buffer++;
	}
	return result;
}

void mlt_strupper(char *buffer)
{
	while (*buffer != '\0') {
		if ((*buffer >= 'a') && (*buffer <= 'z')) *buffer -= 32;
		buffer++;
	}
}

void mlt_strlower(char *buffer)
{
	while (*buffer != '\0') {
		if ((*buffer >= 'A') && (*buffer <= 'Z')) *buffer += 32;
		buffer++;
	}
}

const char **mlt_split(char *buffer, char delim)
{
	int loop, size;

	size   = mlt_count(buffer, delim);
	if (!size)
		return (const char**)NULL;

	char **result = calloc(size * 256, sizeof(char)); 
	loop = 0;
	
	while(1) {
		char *p = mlt_strkey(buffer, loop, delim);
		if (!p)
			break;
	
		*(result + loop) = calloc(256, sizeof(char));
		memcpy(result[loop], p, strlen(p));
		loop++;
		free(p);
	}

	return (const char **)result;
}

const char *mlt_substr(char *buffer, int left, int right)
{
	char *result = NULL;
	int j = 0, n = 0, len = 0;

	if (!buffer)
		return NULL;

	len = strlen(buffer);

	if (left < 1 || right < 1 || left > len || right > len)
		return NULL;

	left--;
	right--;

	result = calloc(len, sizeof(char));
	for (; j < len; j++) {
		if (j >= left && j <= right) {
			result[n++] = buffer[j];
		}
	}

	result = realloc(result, n);
	fprintf(stdout, "%s", result);
	return result;
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
	mlt_substr(buffer, 4, 12);

	fprintf(stdout, "\n-----\n");
	fprintf(stdout, "  func: mlt_substr(buffer, NULL, 22, 29);\nresult: ");
	mlt_substr(buffer, 22, 29);

	mlt_strupper(buffertwo);
	fprintf(stdout, "\n\n-> %s\n", buffertwo); 

	mlt_strlower(buffertwo);
	fprintf(stdout, "\n\n-> %s\n", buffertwo); 

	char **things = mlt_split(buffertwo, ' ');
	fprintf(stdout, "--> %d\n", sizeof(things) / sizeof(things[0]));
	while (*things) {
		fprintf(stdout, "--> '%s'\n", *things);
		things++;
	}
	while (*things) {
		free(*things);
	}
	return 0;
}
#endif

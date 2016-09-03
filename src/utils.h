#include "main.h"

#ifndef _ZTUTILS_H_
#define _ZTUTILS_H_

#define GROUP_OWNERS  0
#define GROUP_PRIVATE 1
#define GROUP_PUBLIC  2

#define GOOGLE "www.google.com"

typedef struct
{
	char command[BUF_MIN];

	int group;
} zt_commands_t;

static zt_commands_t zt_cmd[] = {
	{"!google", GROUP_PUBLIC }

};

#endif

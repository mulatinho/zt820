#include "main.h"

#ifndef _ZTUTILS_H_
#define _ZTUTILS_H_

#define GROUP_MYSELF  0
#define GROUP_OWNER   1
#define GROUP_PRIVATE 2
#define GROUP_PUBLIC  4

#define GOOGLE "www.google.com"

enum { PRIVMSG, ACTION, PING, GLOBAL };
char *type_msg[] = { "PRIVMSG", "ACTION", "PING", "GLOBAL" };

typedef struct
{
	char command[BUF_MIN];

	int type_message, group;

	int (*func)(zt_info *, char *);
} zt_commands_t;

int zt_cmd_google(zt_info *, char *);
int zt_cmd_quote(zt_info *, char *);
int zt_cmd_pong(zt_info *, char *);

static zt_commands_t zt_cmd[] = {
	{"!google", 	PRIVMSG, 	GROUP_PUBLIC,	&zt_cmd_google,	},
	{"!quote", 	PRIVMSG, 	GROUP_PRIVATE,	&zt_cmd_quote,	},
	{"PING :", 	PING, 		GROUP_PUBLIC,	&zt_cmd_pong,	}
};
static int zt_commands_sz = sizeof(zt_cmd) / sizeof(zt_cmd[0]);

#endif

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

	int (*func)(zt_info *, zt_data *, char *);
} zt_commands_t;

char 	*zt_get_output		(char *);
void	zt_get_data			(zt_data *, const char *);

int 	zt_cmd_weather		(zt_info *, zt_data *, char *);
int 	zt_cmd_google		(zt_info *, zt_data *, char *);
int 	zt_cmd_calc			(zt_info *, zt_data *, char *);
int 	zt_cmd_quote		(zt_info *, zt_data *, char *);
int		zt_cmd_quote_find	(zt_info *, zt_data *, char *);
int 	zt_cmd_quote_del	(zt_info *, zt_data *, char *);
int 	zt_cmd_pong			(zt_info *, zt_data *, char *);

static zt_commands_t zt_cmd[] = {
	{"!google", 	PRIVMSG, 	GROUP_PUBLIC,	&zt_cmd_google,	},
	{"!quotes", 	PRIVMSG, 	GROUP_PRIVATE,	&zt_cmd_quote,	},
	{"!quote", 		PRIVMSG, 	GROUP_PRIVATE,	&zt_cmd_quote,	},
	{"!quotenum", 	PRIVMSG, 	GROUP_PRIVATE,	&zt_cmd_quote_find,	},
	{"!quotedel", 	PRIVMSG, 	GROUP_PRIVATE,	&zt_cmd_quote_del,	},
	{"!calc", 		PRIVMSG, 	GROUP_PUBLIC,	&zt_cmd_calc,	},
	{"!weather", 	PRIVMSG, 	GROUP_PUBLIC,	&zt_cmd_weather,	},
	{"PING :", 		PING, 		GROUP_PUBLIC,	&zt_cmd_pong,	}
};
static int zt_commands_sz = sizeof(zt_cmd) / sizeof(zt_cmd[0]);

#endif

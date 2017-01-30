#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#ifndef _ZT820_H_
#define _ZT820_H_

#define BUF_MAX 1024
#define BUF_MED 256
#define BUF_MIN 64

#define MAX_PERMISSIONS 10
#define MAX_CHANNELS 5

enum {
	NICK, REALNAME, USERNAME, SERVER,
	PORT, CHANNELS, PASSWORD, ZT_CMD_SIZE
};

static char *zt_commands[] = {
	"NICK", "REALNAME", "USERNAME", "SERVER",
	"PORT", "CHANNELS", "PASSWORD"
};

typedef struct
{
	int id;

	char nick[BUF_MIN];

	char channel[BUF_MIN];

	char host[BUF_MED];

	char privileges[BUF_MIN];
} zt_auth;

typedef struct
{
	int id;

	char nick[BUF_MIN];

	char channel[BUF_MIN];

	char host[BUF_MED];

	char message[BUF_MAX];

	char channels[MAX_CHANNELS][BUF_MIN];
} zt_request;

typedef struct
{
	int socket;

	int port;

	int feeling;

	char nick[BUF_MIN];

	char username[BUF_MIN];

	char realname[BUF_MED];

	zt_request request;

	zt_auth access_list[MAX_PERMISSIONS];
} zt_info;

char*	mlt_strkey			(char *, int, char);
void 	mlt_strupper		(char *);

int 	zt_interpret		(zt_info *, char *);
int 	zt_feelings_event	(zt_info *, char *);

#endif

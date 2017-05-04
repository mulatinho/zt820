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
#define BUF_MIN 32

#define MAX_CHANNELS 5

enum { 
	NICK, REALNAME, USERNAME, SERVER,
	PORT, CHANNELS, BNC, PASSWORD, NICKSERV, ZT_CMD_SIZE
};
static char *zt_commands[] = {
	"NICK", "REALNAME", "USERNAME", "SERVER", 
	"PORT", "CHANNELS", "BNC", "PASSWORD", "NICKSERV"
};

typedef struct
{
	int id;

	int port; 

	char host[BUF_MED];
	
	char channels[MAX_CHANNELS][BUF_MIN];
} zt_server;

typedef struct
{
	int socket;

	int feeling;

	char bnc[BUF_MIN];

	char nick[BUF_MIN];

	char username[BUF_MIN];

	char realname[BUF_MED];

	char password[BUF_MIN];

	zt_server ircserver;
} zt_info;

struct zt_owners 
{	
	int id;

	char nick[BUF_MIN];
};

char *mlt_strkey(char *buffer, int who, char del);
void mlt_strupper(char *buffer);

int zt_interpret(zt_info *, char *);
int zt_feelings_event(zt_info *, char *);
#endif

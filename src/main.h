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

#include <errno.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#ifndef _ZT820_H_
#define _ZT820_H_

#define BUF_MAX 1024
#define BUF_MED 256
#define BUF_MIN 64

#define MAX_PERMISSIONS 10
#define MAX_CHANNELS 5

typedef struct {
    int socket;
    char *hostname;
    SSL *ssl;
    SSL_CTX *ssl_ctx;
} mlt_sslconn;

enum {
	NICKNAME, REALNAME, USERNAME, SERVER,
	PORT, CHANNELS, BNC, PASSWORD, NICKSERV, ZT_CMD_SIZE
};

static char *zt_commands[] = {
	"NICKNAME", "REALNAME", "USERNAME", "SERVER",
	"PORT", "CHANNELS", "BNC", "PASSWORD", "NICKSERV"
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

    char host[BUF_MED];

	char bnc[BUF_MIN];

	char nick[BUF_MIN];

	char username[BUF_MIN];

	char channels[MAX_CHANNELS][BUF_MED];
	char realname[BUF_MED];

	char password[BUF_MIN];
} zt_info;

typedef struct {
    int id;

    char nick[BUF_MIN];

    char host[BUF_MED];

    char command[BUF_MIN];

    char argument[BUF_MED];

    char message[BUF_MAX];
} zt_data;

char*	mlt_strkey			(char *, int, char);
void 	mlt_strupper		(char *);

void	zt_get_data			(zt_data *, const char *);
int 	zt_interpret		(zt_info *, zt_data *, char *);
int 	zt_feelings_event	(zt_info *, zt_data *);

#endif

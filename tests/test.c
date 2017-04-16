#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_MIN 64
#define BUF_MED 256
#define BUF_MAX 1024

typedef struct data_msg {
    int id;

    char nick[BUF_MIN];

    char host[BUF_MED];

    char command[BUF_MIN];

    char argument[BUF_MED];

    char message[BUF_MAX];
} zt_data;

void zt_get_data(zt_data *data, const char *buffer)
{
    struct data_msg payload = {
        .id = 0, .nick = {0},
        .host = {0}, .command = {0}, .argument = {0}, .message = {0}
    };
    int dots = 0, sep = 0, n = 0;

    for (int i = 0; i < strlen(buffer); i++) {
        if (' ' == buffer[i] && dots < 2) { sep++; n = 0; continue; }

        if (dots == 2) { payload.message[n++] = buffer[i]; }
        else {
            switch (sep) {
                case 1:
                    if ('!' == buffer[i]) { sep++; n = 0; continue; }
                    else { payload.nick[n++] = buffer[i]; }
                    break;
                case 2: payload.host[n++] = buffer[i]; break;
                case 3: payload.command[n++] = buffer[i]; break;
                case 4: payload.argument[n++] = buffer[i]; break;
                default: break;
            }
        }

        if (':' == buffer[i]) { dots++; sep++; n = 0; }
    }

	n = 0; sep = 0;
	//fprintf(stdout, "%s = %d\n", payload.nick, strlen(payload.nick));
	if (1 == strlen(payload.nick)) {
		strcpy(payload.nick, "IRCSERVER");

    	for (int i = 0; i < strlen(buffer); i++) {
			if (' ' == buffer[i] || ':' == buffer[i]) { sep++; n = 0; continue; }
			if (!sep) { payload.command[n++] = buffer[i]; }
			else { payload.host[n++] = buffer[i]; }
		}
	}

	*data = payload;
    fprintf(stdout, "nick: '%s', host: '%s'\ncommand: '%s', argument: '%s', message: '%s'\n",
        payload.nick, payload.host, payload.command, payload.argument, payload.message);
}

int main(int argc, char **argv)
{
    const char* strings[] = {
        ":crudo!~kayaman@unaffiliated/alexandre PRIVMSG #linuxers :!quotenum 2314242",
        ":crudo!~kayaman@unaffiliated/alexandre PRIVMSG #linuxers :porra, dois pontos",
        ":crudo!~kayaman@unaffiliated/alexandre PRIVMSG #linuxers :!quotenum 9",
        ":crudo!~crudo@unaffiliated/alexandre JOIN #vampire :hello",
        ":crudo!~crudo@unaffiliated/alexandre PART #vampire :!joe",
        ":crudo!~crudo@unaffiliated/alexandre PRIVMSG zt820 :!joe",
        "PING :irc.freenode.net",
		":WiZ!jto@tolsun.oulu.fi NICK Kilroy"
    };
    int n_strings = (sizeof(strings) / sizeof(const char*));


    for (int i = 0; i < n_strings; i++) {
        zt_data *in;
		zt_get_data(in, strings[i]);
        fprintf(stdout, "buf: %s, in.nick: %s\n\n", strings[i], in->nick);
	}

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

#define BUF_MIN 64
#define BUF_MED 256
#define BUF_MAX 1024

struct data_msg {
    int id;
    char nick[BUF_MIN];
    char host[BUF_MED];
    char command[BUF_MIN];
    char message[BUF_MAX];
};

void zt_test(const char *buffer)
{
    struct data_msg payload = {
        .id = 0, .nick = {0},
        .host = {0}, .command = {0}, .message = {0}
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
                default: break;
            }
        }

        if (':' == buffer[i]) { dots++; sep++; n = 0; }
    }

    fprintf(stdout, "nick: '%s', host: '%s'\ncommand: '%s', message: '%s'\n\n",
        payload.nick, payload.host, payload.command, payload.message);
}

int main(int argc, char **argv)
{
    const char* strings[] = {
        ":crudo!~kayaman@unaffiliated/alexandre PRIVMSG #linuxers :!quotenum 2314242",
        ":crudo!~kayaman@unaffiliated/alexandre PRIVMSG #linuxers :porra, dois pontos",
        ":crudo!~kayaman@unaffiliated/alexandre PRIVMSG #linuxers :!quotenum 9",
        ":crudo!~crudo@unaffiliated/alexandre JOIN #vampire :hello",
        ":crudo!~crudo@unaffiliated/alexandre PART #vampire :!joe",
        ":crudo!~crudo@unaffiliated/alexandre PRIVMSG zt820 :!joe"
    };
    int n_strings = (sizeof(strings) / sizeof(const char*));


    for (int i = 0; i < n_strings; i++) {
        zt_test(strings[i]);
        fprintf(stdout, "%s\n", strings[i]);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/main.h"
#include "mlt.h"

void zt_get_data(zt_data *, const char *);

#ifdef TEST
int main(void)
{
	mlt_start();

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
        zt_data *in = malloc(sizeof(zt_data));
		zt_get_data(in, strings[i]);

		switch(i) {
			case 0: mlt_streq(in->argument, "#linuxers"); break;
			case 1: mlt_streq(in->nick, "crudo"); break;
			case 2: mlt_streq(in->command, "PRIVMSG"); break;
			default: break;
		}

        free(in);
    }

	mlt_finish();

    return 0;
}
#endif

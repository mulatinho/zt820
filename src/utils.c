#include "utils.h"

char *mlt_strkey(char *buffer, int who, char del);

char *zt_getoutput(char *cmd)
{
	FILE *fp;
	char buf[BUF_MAX] = {0};
	char *ret = (char*)malloc(sizeof(buf));
	int totalbytez = 0;

	memset(ret, 0, sizeof(buf));

	fp = popen(cmd, "r");
	while (fgets(buf, sizeof(buf)-1, fp)) {
		totalbytez+=strlen(buf);

		if (totalbytez)
			ret = (char*)realloc(ret, totalbytez);

		strncat(ret, buf, strlen(buf)-1);
	}
	fclose(fp);

	fprintf(stderr, "\n\n%s\n\n", ret);

	if (!strlen(ret)) {
		free(ret);
		return NULL;
	} else
		return (char*)ret;
}

void zt_clean_string(char *buffer)
{
	if (buffer) {
		for (size_t z = 0; z < strlen(buffer); z++) {
			if (buffer[z] == '\r' || buffer[z] == '\n') { buffer[z] = '\0'; }
		}
	}
}

char *zt_get_line(char *buffer)
{
	char *line = NULL;

	if (buffer) {
		line = (char*)mlt_strkey(buffer, 2, ':');
		if (!line)
			return NULL;
	}

	return line;
}

char *zt_get_cmd(char *buffer)
{
	char *line = NULL;
	char *res  = NULL;

	if ((line = (char*)mlt_strkey(buffer, 2, ':'))) {
		char *res = strchr(line, ' ');

		if (res) {
			char *str = malloc(1024);
			sscanf(line, "%s", str);
			str = realloc(str, strlen(str));
			return str;
		} else {
			return line;
		}
	}

	return line;
}

char *zt_get_args(char *buffer)
{
	char *line = NULL;
    char *res  = NULL;

	if ((line = (char*)mlt_strkey(buffer, 2, ':'))) {
		char *res  = strchr(line, ' ');

		if (!res)
			return NULL;
		else
			return res + 1;
	}

	return NULL;
}

int zt_cmd_pastebin(zt_info *ztinfo, char* string)
{
	char buf[512] = {0};
	char cmd[] = "curl pastebin.com/archives 2>&1 | egrep -o '\"/[A-Za-z0-9]{8}' | sed -n '/[A-Z]/p' | sed -e 's/^\"/pastebin.com\\/raw/g' | while read LINE; do curl http://$LINE 2>/dev/null | egrep -i '(sex|pass|xxx|linux|pussy|hack|rip)' 2>&1 >/dev/null; [ $? -eq 0 ] && echo -en \"www.${LINE}, \"; sleep 0.15; done";

	if (!string) {
		return 1;
	}

	srand(time(NULL));
	if (( (rand() % 1000) + 1 ) > 985) {
		const char* output = zt_getoutput(cmd);
		fprintf(stdout, "hey pastebin!");

		if (output) {
			snprintf(buf, sizeof(buf)-1, "PRIVMSG %s :time for w4r3z -> %s\r\n", ztinfo->ircserver.channels[0], output);
			write(ztinfo->socket, buf, strlen(buf));
		}
	}

	return 0;
}

int zt_cmd_google(zt_info *ztinfo, char *string)
{
	return 0;
}

int zt_cmd_calc(zt_info *ztinfo, char *string)
{
	char buf[BUF_MAX];
	char *q = (char*)mlt_strkey(string, 2, ':');

	if (!q)
		return -1;

	char cmd[BUF_MED];
	snprintf(cmd, sizeof(cmd)-1, "echo \"scale=2;%s\" | bc", strchr(q, ' ') + 1);
	char *output = zt_getoutput(cmd);
	snprintf(buf, sizeof(buf)-1, "PRIVMSG %s :'%s' -> %s.\r\n", ztinfo->ircserver.channels[0], strchr(q, ' ') + 1, output);

	write(ztinfo->socket, buf, strlen(buf));

	return 0;
}

int zt_cmd_weather(zt_info *ztinfo, char *string)
{
	char buf[BUF_MAX] = {0}, cmd[BUF_MAX] = {0};
	char* args = NULL, *output = NULL;

	snprintf(buf, sizeof(buf)-1, "PRIVMSG %s :no such city in records ;)\r\n", ztinfo->ircserver.channels[0]);

	if (!string) {
		fprintf(stdout, "no such string");
		write(ztinfo->socket, buf, strlen(buf));
		return -1;
	}

	args = zt_get_args(string);
	if (!args) {
		fprintf(stdout, "no such args");
		write(ztinfo->socket, buf, strlen(buf));
		return -1;
	}

	fprintf(stdout, "string '%s'\nargs '%s'\n", string, args);
	snprintf(cmd, sizeof(cmd)-1, "curl wttr.in/%s 2>/dev/null | head -n 5 | perl -pe 's/\\x1b\\[[0-9;]*[mG]//g' | tr -cd '[[:alnum:]]._- ' | cat -e | sed 's,  , ,g';", args);
	output = zt_getoutput(cmd);
	fprintf(stdout, "cmd '%s'\noutput '%s'\n", cmd, output);

	if (!output) {
		fprintf(stdout, "no such output");
		write(ztinfo->socket, buf, strlen(buf));
		return -2;
	}
	if (strstr(output, "ERROR")) {
		write(ztinfo->socket, buf, strlen(buf));
		return -3;
	}

	memset(buf, '\0', sizeof(buf));
	snprintf(buf, sizeof(buf)-1, "PRIVMSG %s :%s\r\n", ztinfo->ircserver.channels[0], output);
	write(ztinfo->socket, buf, strlen(buf));

	return 0;
}

int zt_cmd_quote_find(zt_info *ztinfo, char *string)
{
	FILE *fr, *fw;
	char buf[4096] = {0}, response[256] = {0};
	char *strnum = NULL;
	int num, loop;

	strnum = zt_get_args(string);
	if (!strnum) {
		snprintf(response, sizeof(response)-1, "PRIVMSG %s :gimme a real one :>\r\n", ztinfo->ircserver.channels[0]);
		write(ztinfo->socket, response, strlen(response));
		return 1;
	}

	loop = 0;
	num = atoi(strnum);
	if (num) {
		if ((fr = fopen("./data/quotes.txt", "r"))) {
			while (fgets(buf, sizeof(buf)-1, fr)) {
				if (loop == (num-1)) { break; }
				loop++;
			}
			fclose(fr);
		}
		buf[strlen(buf)-1] = '\0';
		snprintf(response, sizeof(response)-1, "PRIVMSG %s :[#%d] %s\r\n", ztinfo->ircserver.channels[0], loop+1, buf);
	}


	if (!strlen(response))
		snprintf(response, sizeof(response)-1, "PRIVMSG %s :gimme a real one :|\r\n", ztinfo->ircserver.channels[0]);

	write(ztinfo->socket, response, strlen(response));

	return 0;
}
int zt_cmd_quote_del(zt_info *ztinfo, char *string)
{
	FILE *fr, *fw;
	char buf[4096] = {0}, response[256] = {0};
	char *strnum = NULL;
	int num, loop;

	strnum = zt_get_args(string);
	if (!strnum) {
		snprintf(response, sizeof(response)-1, "PRIVMSG %s :gimme a real one :>\r\n", ztinfo->ircserver.channels[0]);
		write(ztinfo->socket, response, strlen(response));
		return 1;
	}

	num = atoi(strnum);
	if (!num) {
		snprintf(response, sizeof(response)-1, "PRIVMSG %s :gimme a real one :>\r\n", ztinfo->ircserver.channels[0]);
		write(ztinfo->socket, response, strlen(response));
		return 1;
	}

	loop = 0;
	if ((fr = fopen("./data/quotes.txt", "r"))) {
		if ((fw = fopen("./data/nquotes.txt", "a"))) {
			while (fgets(buf, sizeof(buf)-1, fr)) {
				if (loop != (num-1)) { fprintf(fw, "%s", buf); }
				loop++;
			}
			fclose(fw);
		}
		fclose(fr);
	}

	usleep(400);
	rename("./data/nquotes.txt", "./data/quotes.txt");

	snprintf(response, sizeof(response)-1, "PRIVMSG %s :this quote is revogated!\r\n", ztinfo->ircserver.channels[0]);
	write(ztinfo->socket, response, strlen(response));

	return 0;
}

int zt_cmd_quote(zt_info *ztinfo, char *string)
{
	char buf[BUF_MAX];
	char *q = (char*)mlt_strkey(string, 2, ':');

	fprintf(stdout, "string '%s'\n", q);
	if (strlen(q) > 8) {
		FILE *fp = fopen("./data/quotes.txt", "a");
		char *quote = strchr(q, ' ');
		if (quote) {
			fprintf(fp, "%s\n", quote + 1);
			fclose(fp);

			snprintf(buf, sizeof(buf)-1, "PRIVMSG %s :yess sir.\r\n", ztinfo->ircserver.channels[0]);
		}
	} else {
		FILE *fp = fopen("./data/quotes.txt", "r");
		if (fp) {
			char ibuf[BUF_MAX] = {0};
			int lines = 0;

			while(fgets(ibuf, sizeof(ibuf)-1, fp)) { lines++; }

			int num = rand() % lines;
			fprintf(stdout, "lines  %d , num %d\n", lines, num);
			lines = 0;

			rewind(fp);
			while(fgets(ibuf, sizeof(ibuf)-1, fp)) {
				if (num == lines) {
					snprintf(buf, sizeof(buf)-1,
						"PRIVMSG %s :[#%d] %s\r\n", ztinfo->ircserver.channels[0], (num+1), ibuf);
					break;
				}
				lines++;
			}
			fclose(fp);
		}
	}

	if (!strlen(buf)) {
		snprintf(buf, sizeof(buf)-1,
			"PRIVMSG %s :hey joe\r\n", ztinfo->ircserver.channels[0]);
	}

	fprintf(stdout, "%s", buf);
	write(ztinfo->socket, buf, strlen(buf));

	return 0;
}

int zt_cmd_pong(zt_info *ztinfo, char *string)
{
	char buf[BUF_MAX];
	char *servname = (char*)mlt_strkey(string, 1, ' ');

	snprintf(buf, sizeof(buf)-1, "PONG %s\r\n", servname);
	fprintf(stdout, "%s:. server is pinging me, pong him back\n%s\n", string, buf);

	write(ztinfo->socket, buf, strlen(buf));

	return 0;
}

int zt_interpret(zt_info *ztinfo, char *string)
{
	int found = 0;

	if (!string) { return 1; }

	zt_clean_string(string);

	const char *ptr = zt_get_cmd(string);
	if (ptr) {
		for (int i = 0; i < zt_commands_sz; i++) {
			//DEBUG fprintf(stdout, ". '%s' %d, '%s' %d\n", ptr, strlen(ptr), zt_cmd[i].command, strlen(zt_cmd[i].command));
			if (!strncmp(zt_cmd[i].command, ptr, strlen(zt_cmd[i].command))) {
				zt_cmd[i].func(ztinfo, string);
				found++;
			}
		}
	}

	if (!found) {
		for (int i = 0; i < zt_commands_sz; i++) {
			//DEBUG fprintf(stdout, ". '%s' %d, '%s' %d\n", string, strlen(string), zt_cmd[i].command, strlen(zt_cmd[i].command));
			if (strstr(string, zt_cmd[i].command)) {
				zt_cmd[i].func(ztinfo, string);
				found++;
			}
		}
	}

	if (!found)
		zt_cmd_pastebin(ztinfo, string);

	usleep(500);
}

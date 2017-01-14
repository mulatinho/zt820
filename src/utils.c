#include "utils.h"

char *mlt_strkey(char *buffer, int who, char del);

char *zt_getoutput(char *cmd)
{
	FILE *fp;
	char buf[256];
	char *ret = (char*)malloc(sizeof(buf));
	int totalbytez = 0;

	memset(ret, 0, sizeof(buf));

	fp = popen(cmd, "r");
	while (fgets(buf, sizeof(buf)-1, fp)) {
		totalbytez+=strlen(buf);

		if (totalbytez > 12)
			ret = (char*)realloc(ret, totalbytez);

		strncat(ret, buf, strlen(buf)-1);
	}
	fclose(fp);

	if (!strlen(ret)) {
		free(ret);
		return NULL;
	} else
		return (char*)ret;
}

void zt_clean_string(char *buffer) 
{
	for (int z = 0; z < strlen(buffer); z++) {
		if (buffer[z] == '\r' || buffer[z] == '\n') { buffer[z] = '\0'; }
	}

	buffer = realloc(buffer, strlen(buffer));
}

char *zt_get_line(char *buffer)
{
	char *line = NULL;

	line = (char*)mlt_strkey(buffer, 2, ':');
	if (!line)
		return NULL;

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

int zt_cmd_google(zt_info *ztinfo, char *string)
{
	struct sockaddr_in inet;
	struct hostent *host;
	char buf[BUF_MAX], format[BUF_MAX];
	int nsoq, bytes;
	char *q;

	/* if ((nsoq = socket(AF_INET,SOCK_STREAM,0)) == -1)
		return -1;

	inet.sin_port   = htons(80);
	inet.sin_family = AF_INET;

	if ((host=gethostbyname(GOOGLE)) == NULL)
		return -2;

	memcpy(&inet.sin_addr, host->h_addr_list[0], host->h_length);

	if (connect(nsoq, (struct sockaddr *)&inet, sizeof(inet)) == -1)
		return -3;

	q = (char*)mlt_strkey(string, 4, ' ');

	if (q)
		q[strlen(q)-2] = '\0';
	else
		return -1;

	snprintf(format, sizeof(format)-1, "GET /search?q=%s HTTP/1.0\r\n", q);
	send(nsoq, format, strlen(format), 0);
	fprintf(stdout, ":. sending search to google.. '%s'\n", format);


	sleep(1);
	while ((bytes = recv(nsoq, format, BUF_MAX-1, 0))) {
		int l = 0, j = 0;
		format[bytes] = '\0';
		fprintf(stdout, ":=> %s\n", format);

		for (size_t i=0; i < strlen(format); i++) {
			if (format[i] == '2' && format[i+14] == 'r' && format[i+15] == 'e' && format[i+16] == 'f')
			{
				l++; i+=19;
			}

			if (l)
			{
				if (format[i] == '"')
				{
					buf[j] = '\0';

					snprintf(buf, sizeof(buf)-1, "PRIVMSG %s :%s\r\n", ztinfo->ircserver.channels[0], buf);
					send(nsoq, buf, strlen(buf), 0);
					l=0; j=0; sleep(1);
				}
				else
				{
					buf[j] = format[i];
					j++;
				}
			}
		}

		memset(format, '\0', sizeof(format));
	}

	close(nsoq); */
	return 0;
}

int zt_cmd_calc(zt_info *ztinfo, char *string)
{
	char buf[BUF_MAX];
	char *q = (char*)mlt_strkey(string, 2, ':');

	if (q)
		q[strlen(q)-2] = '\0';
	else
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
	char buf[BUF_MAX], cmd[BUF_MED];
	char *args = zt_get_args(string);

	if (!args)
		return -1;

	*(args + (strlen(args)-2)) = '\0';
	snprintf(cmd, sizeof(cmd)-1, "curl wttr.in/%s 2>/dev/null | head -n 5 | egrep '(Partly|City|C)' | paste -s -d, | sed -e 's/\\x1B\\[[0-9;]*[JKmsu]//g' | cat -A | sed -e 's/M-.//g; s/  / /g; s/@//g' | tr -d '[:punct:]'", args);
	char *output = zt_getoutput(cmd);
	fprintf(stdout, "%s\n", output);

	/* if (output) {
		snprintf(buf, sizeof(buf)-1, "PRIVMSG %s :%s\r\n", output);
		write(ztinfo->socket, buf, strlen(buf));
	} */

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
		snprintf(response, sizeof(response)-1, "PRIVMSG %s :%s\r\n", ztinfo->ircserver.channels[0], buf);
	}


	write(ztinfo->socket, response, strlen(response));
	snprintf(response, sizeof(response)-1, "PRIVMSG %s :gimme a real one :|\r\n", ztinfo->ircserver.channels[0]);

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

	zt_clean_string(q);
	fprintf(stdout, "string '%s'\n", q);
	if (strlen(q) > 8) {
		q[strlen(q)-1] = '\0';

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
	char buf[BUF_MAX] = {0};

	if (!string) { return 1; }

	for (int i = 0; i < zt_commands_sz; i++) {
		char *ptr = zt_get_cmd(string);
		if (strlen(ptr) > 3) {
			for (int z = 0; z < strlen(ptr); z++) {
				if (ptr[z] == '\r' || ptr[z] == '\n') { ptr[z] = '\0'; }
			}

			fprintf(stdout, ". '%s' %d, '%s' %d\n", ptr, strlen(ptr), zt_cmd[i].command, strlen(zt_cmd[i].command));
			if (!strncmp(zt_cmd[i].command, ptr, strlen(zt_cmd[i].command))) {
				zt_cmd[i].func(ztinfo, string);
			}
		}
	}

	usleep(500);
}

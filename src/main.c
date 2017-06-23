#include "main.h"

int zt_check_configcmd(char *cmd)
{
	if (!cmd)
		return -1;

	mlt_strupper(cmd);
	for (int i = 0; i < ZT_CMD_SIZE; i++) {
		if (!strncmp(zt_commands[i], cmd, strlen(zt_commands[i])))
			return i;
	}

	return -1;
}

FILE *zt_check_file(void)
{
	FILE *fp;
	char *HOME_ENV = getenv("HOME");
	char *testfile = calloc(BUF_MED, sizeof(char));

	snprintf(testfile, BUF_MED-1, "./data/config");
	if (!(fp = fopen(testfile, "r"))) {
		snprintf(testfile, BUF_MED-1, "%s/.zt820_config", HOME_ENV);
		if (!(fp = fopen(testfile, "r"))) {
			snprintf(testfile, BUF_MED-1, "/etc/zt820/config");
			if (!(fp = fopen(testfile, "r"))) {
				free(testfile);
				return NULL;
			}
		}
	}

	free(testfile);
	return fp;
}

int zt_read_conf(zt_info *ztinfo)
{
	FILE *zt_conf;
	char buffer[BUF_MAX];

	if (!(zt_conf = zt_check_file())) {
		fprintf(stderr, ":. i can't find any 'config' file\n");
		exit(-1);
	}

	while (fgets(buffer, sizeof(buffer)-1, zt_conf)) {
		buffer[strlen(buffer)-1] = '\0';

		if (buffer[0] == '#')
			continue;
		else {
			char *nbuf     = strdup(buffer);
			char *property = NULL;
			char *value    = NULL;

			if (strchr(nbuf, '"') == NULL) {
				property = mlt_strkey(nbuf + 0, 0, ' ');
				value    = mlt_strkey(nbuf + 0, 1, ' ');
			} else {
				property = mlt_strkey(buffer, 0, ' ');
				value    = mlt_strkey(buffer, 1, '"');
			}

			int idcmd = zt_check_configcmd(property);
			if (idcmd != -1 && value) {
				switch (idcmd) {
				case NICK:
					memcpy(ztinfo->nick, value, BUF_MIN-1);
					break;
				case REALNAME:
					memcpy(ztinfo->realname, value, BUF_MIN-1);
					break;
				case USERNAME:
					memcpy(ztinfo->username, value, BUF_MIN-1);
					break;
				case SERVER:
					memcpy(ztinfo->host, value, BUF_MIN-1);
					break;
				case PORT:
					ztinfo->port = atoi(value);
					break;
				case CHANNELS:
					memcpy(ztinfo->channels[0], value, BUF_MIN-1);
					break;
				case PASSWORD:
					memcpy(ztinfo->password, value, BUF_MIN-1);
					break;
				}
			}
			fprintf(stdout, "-> '%s' '%s' '%s' '%s' '%d' '%s'\n", ztinfo->nick, ztinfo->realname, ztinfo->username,
				ztinfo->host, ztinfo->port, ztinfo->channels[0]);

			free(property);
			free(value);
		}


	}

	fclose(zt_conf);
	return 0;
}

void zt_get_data(zt_data *data, const char *buffer)
{
    zt_data payload = {
        .id = 0, .nick = {0},
        .host = {0}, .command = {0}, .argument = {0}, .message = {0}
    };
    int dots = 0, sep = 0, n = 0;

    for (size_t i = 0; i < strlen(buffer); i++) {
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

    	for (size_t i = 0; i < strlen(buffer); i++) {
			if (' ' == buffer[i] || ':' == buffer[i]) { sep++; n = 0; continue; }
			if (!sep) { payload.command[n++] = buffer[i]; }
			else { payload.host[n++] = buffer[i]; }
		}
	}

	zt_clean_string(payload.nick);
	zt_clean_string(payload.host);
	zt_clean_string(payload.command);
	zt_clean_string(payload.argument);
	zt_clean_string(payload.message);

	*data = payload;
    fprintf(stdout, "nick: '%s', host: '%s'\ncommand: '%s', argument: '%s', message: '%s'\n",
        payload.nick, payload.host, payload.command, payload.argument, payload.message);
}

int zt_event_loop(zt_info *ztinfo, char *buffer)
{
	zt_data *data = malloc(sizeof(zt_data));

	zt_get_data(data, buffer);

	zt_feelings_event(ztinfo);
	zt_interpret(ztinfo, buffer);

	free(data);

	return 0;
}

int zt_create_server(zt_info *ztinfo)
{
	int rbytes, init = 0, sw, ret;
	char buf[BUF_MAX], sbuf[BUF_MED];
	struct hostent *host;
	struct sockaddr_in server;
	struct pollfd *serverpoll = malloc(sizeof(struct pollfd));

	sw = 1; ret = init = 0;

	server.sin_port = htons(ztinfo->port);
	server.sin_family = AF_INET;

	if ((host = gethostbyname(ztinfo->host)) == NULL)
		return -0xdead;

	memcpy(&server.sin_addr, host->h_addr_list[0], host->h_length);

	if ((ztinfo->socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return -1;

	setsockopt(ztinfo->socket, SOL_SOCKET, SO_KEEPALIVE, (char *) &sw, sizeof (sw));

	if (connect(ztinfo->socket, (struct sockaddr*)&server, sizeof(struct sockaddr)) == -1)
		return -3;

	serverpoll->fd = ztinfo->socket;
	serverpoll->events = POLLIN|POLLOUT;

	fprintf(stderr, ":. server going to wait events..\n");
	while (1) {
		ret = poll(serverpoll, 1, 25000);
		if (ret < 0) {
			fprintf(stderr, "poll: requested event failed!\n");
			return -2;
		}

		if (serverpoll->revents & POLLIN) {
			rbytes = read(serverpoll->fd, buf, sizeof(buf)-1);
			if (!rbytes) {
				close(serverpoll->fd);
				serverpoll->fd = -1;
				serverpoll->events = -1;

				fprintf(stdout, ":. connection to server is closed\n");
				break;
			}

			if (rbytes) {
				buf[rbytes] = '\0';

				fprintf(stdout, "-> %s\n", buf);
				if (!init) {
					if (strstr(buf, "your hostname")) {
						memset(sbuf, '\0', sizeof(sbuf));

						fprintf(stdout, ":. sending username...\n");
						snprintf(sbuf, sizeof(sbuf), "USER %s 0 * :%s\r\n", ztinfo->username, ztinfo->realname);
						write(serverpoll->fd, sbuf, strlen(sbuf));

						fprintf(stdout, ":. sending nickname...\n");
						snprintf(sbuf, sizeof(sbuf), "NICK %s\r\n", ztinfo->nick);
						write(serverpoll->fd, sbuf, strlen(sbuf));
					}
					init++;
				}
				if (strstr(buf, ":End of /MOTD")) {
					snprintf(sbuf, sizeof(sbuf), "MSG NickServ IDENTIFY %s\r\n", ztinfo->password);
					write(serverpoll->fd, sbuf, strlen(sbuf));

					snprintf(sbuf, sizeof(sbuf), "JOIN %s\r\n", ztinfo->channels[0]);
					write(serverpoll->fd, sbuf, strlen(sbuf));
				}

				zt_event_loop(ztinfo, buf);
			}

			memset(buf, '\0', sizeof(buf));
			rbytes = 0;
		}
		usleep(500000);
	}

	return 0;
}

int main(void)
{
	zt_info *conf = calloc(1, sizeof(zt_info));

	if (!zt_read_conf(conf))
		zt_create_server(conf);

	return 0;
}

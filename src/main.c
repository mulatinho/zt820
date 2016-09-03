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
					memcpy(ztinfo->ircserver.host, value, BUF_MIN-1);
					break;
				case PORT:
					ztinfo->ircserver.port = atoi(value);
					break;
				case CHANNELS:
					memcpy(ztinfo->ircserver.channels[0], value, BUF_MIN-1);
					break;
				}
			}
			fprintf(stdout, "-> '%s' '%s' '%s' '%s' '%d' '%s'\n", ztinfo->nick, ztinfo->realname, ztinfo->username,
				ztinfo->ircserver.host, ztinfo->ircserver.port, ztinfo->ircserver.channels[0]);

			free(property);
			free(value);
		}

		
	}

	fclose(zt_conf);
	return 0;
}

int zt_event_loop(zt_info *ztinfo, char *buffer)
{
	zt_feelings_check();
	zt_interpret(ztinfo, buffer);

	return 0;
}

int zt_create_server(zt_info *ztinfo)
{
	int rbytes, sw, ret;
	char buf[BUF_MAX], sbuf[BUF_MED];
	struct hostent *host;
	struct sockaddr_in server;
	struct pollfd *serverpoll = malloc(sizeof(struct pollfd));

	sw = 1; ret = 0;

	server.sin_port = htons(ztinfo->ircserver.port);
	server.sin_family = AF_INET;
	
	if ((host = gethostbyname(ztinfo->ircserver.host)) == NULL)
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
		ret = poll(serverpoll, 1, 0);
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
				if (strstr(buf, "Couldn't look up your hostname")) {
					memset(sbuf, '\0', sizeof(sbuf));
					//sleep(2);

					fprintf(stdout, ":. sending username...\n");
					snprintf(sbuf, sizeof(sbuf), "USER zt_soul 0 * :zt_soul. The irony of destiny\r\n");
					write(serverpoll->fd, sbuf, strlen(sbuf));
					
					//usleep(200);

					fprintf(stdout, ":. sending nickname...\n");
					snprintf(sbuf, sizeof(sbuf), "NICK zt_soul\r\n");
					write(serverpoll->fd, sbuf, strlen(sbuf));
				} 
				if (strstr(buf, "zt_soul :End of /MOTD")) {
					/*fprinf(stdout, ":. sending nickname...\n");
					snprintf(sbuf, sizeof(sbuf), "NICKSERV IDENTIFY 820zt\r\n");
					write(serverpoll->fd, sbuf, strlen(sbuf)); */
					snprintf(sbuf, sizeof(sbuf), "JOIN %s\r\n", ztinfo->ircserver.channels[0]);
					write(serverpoll->fd, sbuf, strlen(sbuf));
					/* sleep(2);
					snprintf(sbuf, sizeof(sbuf), "CHANSERV OP %s\r\n", CHANNEL);
					write(serverpoll->fd, sbuf, strlen(sbuf)); */
				}
					
				zt_event_loop(ztinfo, buf);
			}
					
			memset(buf, '\0', sizeof(buf));
			rbytes = 0;
		}
		usleep(50);
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

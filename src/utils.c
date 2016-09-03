#include "utils.h"

char *mlt_strkey(char *buffer, int who, char del);
int zt_cmd_google(zt_info *ztinfo, char *string)
{
	struct sockaddr_in inet;
	struct hostent *host;
	char buf[BUF_MAX], format[BUF_MAX];
	int nsoq, bytes;
	char *q;

	if ((nsoq = socket(AF_INET,SOCK_STREAM,0)) == -1)
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
		q[strlen(q)] = '\0';
	else
		return -1;

	snprintf(format, sizeof(format)-1, "GET /search?q=%s HTTP/1.0\r\n", q); 
	send(nsoq, format, strlen(format), 0);
	fprintf(stdout, ":. sending search to google..\n");

	while ((bytes = recv(nsoq,format,BUF_MAX-1,0)) <= 0) {
		int l = 0, j = 0; 
		format[bytes] = '\0';
		fprintf(stderr, ":=> %s\n", format);

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

	return 0;
}

int zt_cmd_quote(zt_info *ztinfo, char *string)
{
	char buf[BUF_MAX];
	snprintf(buf, sizeof(buf)-1, "PRIVMSG %s :!quote surprise, not implemented yet.\r\n", ztinfo->ircserver.channels[0]);

	write(ztinfo->socket, buf, strlen(buf));
	fprintf(stdout, "!quote surprise, not implemented yet.\n");

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
	char buf[BUF_MAX];

	for (int i = 0; i < zt_commands_sz; i++) {
		if (strstr(string, zt_cmd[i].command))
			zt_cmd[i].func(ztinfo, string);
	}

	usleep(500);
}

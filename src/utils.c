#include "utils.h"

char *mlt_strkey(char *buffer, int who, char del);
int zt_interpret(zt_info *ztinfo, char *string)
{
	struct sockaddr_in inet;
	struct hostent *host;
	char buf[BUF_MAX], format[BUF_MAX];
	int nsoq, bytes;
	char *q;

	if (strstr(string, "PING :")) {
		fprintf(stdout, ":. server is pinging me, pong him back\n");
		q = (char*)mlt_strkey(string, 1, ' ');
		snprintf(buf, sizeof(buf)-1, "PONG %s\r\n", q);

		write(ztinfo->socket, buf, strlen(buf));
	}

	if (strstr(string,":!google ")) {
		if ((nsoq = socket(AF_INET,SOCK_STREAM,0)) == -1)
			return -1;
	
		inet.sin_port   = htons(80);
		inet.sin_family = AF_INET;
		
		if ((host=gethostbyname(GOOGLE)) == NULL)
			return -0x121;
	
		memcpy(&inet.sin_addr, host->h_addr_list[0], host->h_length);

		if (connect(nsoq, (struct sockaddr *)&inet, sizeof(inet)) == -1)
			return -1;

		q = (char*)mlt_strkey(string, 4, ' ');

		if (q)
			q[strlen(q)] = '\0';
		else
			return -1;

		snprintf(format, sizeof(format)-1, "GET /search?q=%s HTTP/1.0\r\n", q); 
		send(nsoq, format, strlen(format), 0);
		fprintf(stdout, ":. sending search to google..\n");

		while (1) 
		{
			if ((bytes=recv(nsoq,format,BUF_MAX-1,0)) <= 0)
				break;

			if (bytes) {
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
			}

			memset(format, '\0', sizeof(format));
			bytes = 0;
		} 

		usleep(20);
	}

	return 0;
}

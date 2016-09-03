#include "zt820.h"

char *
strkey(char *buf, int who, short del)
{
	char *str = (char*)malloc(strlen(buf));
	short ch, i, f=0, x=0, loop=0;
	
	if (!str)
		return NULL;

	for (i=0;i<strlen(buf);i++) 
	{
		ch = *(buf+i);
		
		if (ch==del && !i && !who)
			return NULL;
		
		if (ch==del) 
			loop++; 
		
		if (loop==who) 
		{
			if (f!=0 || i==0) 
			{
				*(str+x) = *(buf+i); x++;
			}
			f++; 
		}
	}
	x++;
	*(str+x)='\0';
	str=(char*)realloc(str,x);
	return (char*)str;
}


char *
getoutput(char *cmd)
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

	if (!strlen(ret))
		return NULL;
	else
		return (char*)ret;
}

char *
zt_ia_hello(void)
{
	int i, total;
	char buf[BUF_MAX];
	char *msg[] = {
		"Opa!",
		"Bora mermão!!",
		"Oi",
		"E aee!",
		"Olá!"
	};

	i=0;total=0;srand(time(NULL));
	total = sizeof(msg) / sizeof(msg[0]);

	i = rand() % total;
	fprintf(stderr, "total:%d i:%d msg:%s\n", total, i, msg[i]);

	return msg[i];
}

int zt_ia(int soq, char *string)
{
	char buf[BUF_MAX];
	char *answer, *p;
	int talktome, feelings, question;

	talktome = 0; question = 0; feelings = 0;

	// PROGRAMMING = 0 ; FELIZ = 1, 3, 5 ; TRISTE = 2; RAIVA = 4
	srand(time(NULL));
	feelings = rand() % 6;

	if (strstr(string, "zt820"))
	{
		talktome++;
	}
	else if (strchr(string, '?'))
	{
		question++;
	}

	if (!feelings)
	{
		snprintf(buf, sizeof(buf)-1, "PRIVMSG %s :Okay, hoje estou programando.\r\n\0", CHANNEL);	
	}
	else if (feelings == 1 || feelings == 3 || feelings == 5)
	{
		snprintf(buf, sizeof(buf)-1, "PRIVMSG %s :Okay, hoje estou feliz.\r\n\0", CHANNEL);	
	}
	else if (feelings == 2)
	{
		snprintf(buf, sizeof(buf)-1, "PRIVMSG %s :Estou triste! :~\r\n\0", CHANNEL);
	}
	else if (feelings == 4)
	{
		snprintf(buf, sizeof(buf)-1, "PRIVMSG %s :To com raiva porra! :/\r\n\0", CHANNEL);
	}

	send(soq, buf, strlen(buf), 0);

	if (question)
	{
		answer = zt_ia_hello();
		snprintf(buf, sizeof(buf)-1, "PRIVMSG %s :%s\r\n\0", CHANNEL, answer);
		send(soq, buf, strlen(buf), 0);
	}
	
	return 0;
}

int zt_interpret(int soq, char *string)
{
	struct sockaddr_in inet;
	struct hostent *hst;
	char format[BUF_MAX], buf[BUF_MIN], tmp[BUF_MIN];
	int i, j, l, v, ran, nsoq, loop, bytes, wr_log, randnum;
	char *p, *q, *x;
	FILE *fp, *fw;

	loop=0; wr_log=0; v=0;
	/*if ((fw = fopen("channel.log", "a"))) {
		wr_log++;
	}*/
	
	if (strstr(string, "PING :")) 
	{
		q = strkey(string, 1, ' ');
		snprintf(tmp, sizeof(tmp)-1, "PONG %s\r\n", q);

		write(soq, tmp, strlen(tmp));
		v++;
	}

	if (strstr(string,":!google ")) 
	{
		if ((nsoq = socket(AF_INET,SOCK_STREAM,0)) == -1)
			return -1;
	
		inet.sin_port   = htons(80);
		inet.sin_family = AF_INET;
		
		if ((hst=gethostbyname(GOOGLE)) == NULL)
			return -0x121;
	
		memcpy(&inet.sin_addr, hst->h_addr_list[0], hst->h_length);

		if (connect(nsoq, (struct sockaddr *)&inet, sizeof(inet)) == -1)
			return -1;

		q = strkey(string, 4, ' ');

		if (q)
			q[strlen(q)] = '\0';
		else
			return -1;

		snprintf(format, sizeof(format)-1, "GET /search?q=%s HTTP/1.0\r\n\0", q); 
		send(nsoq, format, strlen(format), 0); j=0;

		while (1) 
		{
			if ((bytes=recv(nsoq,format,BUF_MAX-1,0)) <= 0)
				return -0x212;

			if (bytes) 
			{
				format[bytes] = '\0';
				fprintf(stderr, ":=> %s\n", format);
				l = 0; j = 0;

				for (i=0;i<strlen(format);i++) 
				{
					if (format[i] == '2' && format[i+14] == 'r' && format[i+15] == 'e' && format[i+16] == 'f') 
					{
						l++; i+=19; 
					}

					if (l) 
					{
						if (format[i] == '"') 
						{
							tmp[j] = '\0';
						
							snprintf(buf, sizeof(buf)-1, "PRIVMSG %s :%s\r\n\0", CHANNEL, tmp);
							send(soq, buf, strlen(buf), 0); 
							l=0; j=0; sleep(1);
						} 
						else 
						{
							tmp[j] = format[i];
							j++;
						}
					}
				} 
			}

			memset(format,'\0',sizeof(format));
			bytes=0;
		} 

		v++;
		sleep(20);
	}

	if (strstr(string, "PRIVMSG") && strstr(string, ":d10")) 
	{
		randnum = 0; memset(buf, 0, sizeof(buf));
		q = strkey(string, 0, ' '); x = strkey(q, 0, '!');
		p = strkey(string, 4, ' '); srand(time(NULL));
		
		if (p) 
		{
			*(p + strlen(p)) = '\0';
			randnum = atoi(p);

			if (randnum > 20)
				randnum = 20;
			else if (randnum < 1)
				randnum = 1;
			
			for (i=0; i<randnum; i++) 
			{
				ran = 1 + (rand()%10);

				if (!buf)
					snprintf(buf, sizeof(buf)-1, "%d ", ran);
				else 
				{
					snprintf(tmp, sizeof(tmp)-1, "%d ", ran);
					strcat(buf, tmp);
				}
			}
		}

		if (buf) 
		{
			if (strstr(string, CHANNEL)) 
			{
				snprintf(format, sizeof(format)-1, "PRIVMSG %s :%s\r\n", CHANNEL, buf);
			} 
			else 
			{
				*(x+0) = ' ';
				snprintf(format, sizeof(format)-1, "PRIVMSG%s :%s\r\n", x, buf);
			}
			
			write(1, format, strlen(format));
			write(soq, format, strlen(format));
		}
		v++;
	}

	if (!v)
		zt_ia(soq, string);

//	if (!wr_log) {
		fprintf(stderr, "%s", string);
/*	} else {
		fprintf(fw, "%s", string); 
	} 
	
	if (fw)
		fclose(fw); */

	return 0;
}

int main(void)
{
	int rbytes, users, smax;
	int sw, ret, soq, newsoq;
	char buf[BUF_MAX], sbuf[BUF_MIN];
	struct hostent *host;
	struct sockaddr_in server;
	struct pollfd serverpoll[2];
		
	sw = 1; ret = 0; users = 0; smax = 0;

	server.sin_port = htons(6667);
	server.sin_family = AF_INET;
	
	if ((host = gethostbyname("irc.freenode.net")) == NULL)
		return -0xdead;
	
	memcpy(&server.sin_addr, host->h_addr_list[0], host->h_length);

	if ((soq = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
		return -1;

	setsockopt(soq, SOL_SOCKET, SO_KEEPALIVE, (char *) &sw, sizeof (sw));

	if (connect(soq, (struct sockaddr*)&server, sizeof(struct sockaddr)) == -1)
		return -3;

	serverpoll[0].fd = soq;
	serverpoll[0].events = POLLIN|POLLOUT;

	fprintf(stderr, "server going to wait events..\n");
	while (1) {
		ret = poll(serverpoll, smax+1, 0);
		if (ret < 0) {
			fprintf(stderr, "poll: requested event failed!\n");
			return -2;
		}

		if (serverpoll[0].revents & POLLIN) {
			rbytes = read(serverpoll[users].fd, buf, sizeof(buf)-1);
			if (!rbytes) {
				close(serverpoll[users].fd);
				serverpoll[0].fd = -1;
				serverpoll[0].events = -1;

				smax--;
			}
			
			if (rbytes) {
				buf[rbytes] = '\0';

				if (strstr(buf, "your hostname")) {
					snprintf(sbuf, sizeof(sbuf), "USER zt820 zt820 zt820 :The irony of destiny\r\n");
					write(serverpoll[0].fd, sbuf, strlen(sbuf));
					snprintf(sbuf, sizeof(sbuf), "NICK zt820\r\n");
					write(serverpoll[0].fd, sbuf, strlen(sbuf));
				}
				if (strstr(buf, "376 zt820 :End") || strstr(buf, "zt820 :End of /MOTD")) {
					snprintf(sbuf, sizeof(sbuf), "NICKSERV IDENTIFY 820zt\r\n");
					write(serverpoll[0].fd, sbuf, strlen(sbuf));
					snprintf(sbuf, sizeof(sbuf), "JOIN %s\r\n", CHANNEL);
					write(serverpoll[0].fd, sbuf, strlen(sbuf));
					sleep(2);
					snprintf(sbuf, sizeof(sbuf), "CHANSERV OP %s\r\n", CHANNEL);
					write(serverpoll[0].fd, sbuf, strlen(sbuf));
				}
					
				zt_interpret(serverpoll[0].fd, buf);
			}
					
			memset(buf, 0, rbytes);
			rbytes = 0;
		}
		usleep(3);
	}
	
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in ser_addr;
	char message[1024];
	int read_len;

	if(argc!=3)
	{
		printf("Usage : %s <IP> <port> \n", argv[0]);
		exit(1);
	}
	
	//socket create
	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	if(sock == -1)
		error_handling("socket() error");

	memset(&ser_addr, 0, sizeof(ser_addr));
	
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = inet_addr(argv[1]);
	ser_addr.sin_port = htons(atoi(argv[2]));
	
	//demand connecting
	if(connect(sock, (struct sockaddr *)&ser_addr, sizeof(ser_addr))==-1)
		error_handling("connect() error");
	else
		puts("Connected.............");

	//communication
	while(1)
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, 1024, stdin);

		if(!strcmp(message,"q\n")|| !strcmp(message,"Q\n"))
			break;

		write(sock, message, strlen(message));
		read_len = read(sock, message, 1023);

		if(read_len == -1)
			error_handling("read() error");
	
		message[read_len]=0;

		printf("Message from server : %s",message);
	}
	//disconnect
	close(sock);
	
	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr);
	fputc('\n', stderr);
	exit(1);
}

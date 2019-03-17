#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);


int main(int argc, char *argv[])
{
	int ser_sock, clt_sock;
	struct sockaddr_in ser_addr,clt_addr;
	socklen_t clt_addr_len;

	char message[1024];
	int read_len, i=0;

	if(argc!=2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	//socket create
	ser_sock = socket(PF_INET, SOCK_STREAM, 0);
	
	if(ser_sock == -1)
		error_handling("socket() error");

	memset(&ser_addr, 0, sizeof(ser_addr));

	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	ser_addr.sin_port = htons(atoi(argv[1]));
	
	//IP, PORT bind
	if(bind(ser_sock, (struct sockaddr*) &ser_addr, sizeof(ser_addr))==-1)
		error_handling("bind() error");
	
	//listen
	if(listen(ser_sock,5) ==-1)
		error_handling("listen() error");

	//accept
	clt_addr_len = sizeof(clt_addr);
	
	while(1)
	{

		clt_sock= accept(ser_sock, (struct sockaddr*)&clt_addr, &clt_addr_len);
		if(clt_sock == -1)
			error_handling("accept() error");
		else {
			i++;
			printf("Connected Client %d \n",i);
		}
	
	//communication
		while((read_len=read(clt_sock, message, 1024))!=0)
			write(clt_sock, message, sizeof(message));
		
	//disconnect
		close(clt_sock);
	}
	close(ser_sock);

	return 0;
}


void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

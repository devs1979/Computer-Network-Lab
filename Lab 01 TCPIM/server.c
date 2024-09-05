#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
int main()
{

	char *ip="127.0.0.1";
	int port=5566;
	int server_sock,client_sock;
	struct sockaddr_in server_addr, client_addr;
	//socklent_t addr_size;
       socklen_t addr_size;
	char buffer[1024];
	int n;
//create TCP server socket

server_sock=socket(AF_INET,SOCK_STREAM,0); //Dgram
if(server_sock<0)
{
	perror("[1] socket  error ");
        exit(1);
}
	printf("[1] TCP server socket created \n");
//provide information
	memset(&server_addr,'\0',sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=port;
	server_addr.sin_addr.s_addr=inet_addr(ip);

//bind address in port no.

	n=bind(server_sock,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(n<0)
	{
		perror("[2] bind error");
		exit(1);
	}
printf("[+] bind to the port number %d\n",port);


//listen

listen(server_sock,5);
printf("listenning \n");
while(1)
{	
	addr_size=sizeof(client_addr);
	client_sock=accept(server_sock,(struct sockaddr*)&client_addr, &addr_size);
	printf("[++] client connection \n");
}
return 0;
}\


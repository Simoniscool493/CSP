#include "unp.h"
#include <time.h>

int main(int argc,char **argv)
{
	int listenfd,connfd,n;
	struct sockaddr_in servaddr,cliaddr;
	char buff[MAXLINE];
	char cmd[16];
 	char path[64];
	char vers[16];

	socklen_t len = sizeof(cliaddr);

	if(argc!=2)
	{
		err_quit("usage: <Program Name> <Port No.>");
	}

	listenfd = Socket(AF_INET,SOCK_STREAM,0);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	Bind(listenfd,(SA *) &servaddr,sizeof(servaddr));

	Listen(listenfd,LISTENQ);

	for( ; ; )
	{
		connfd = Accept(listenfd,(SA *) &cliaddr,&len);

		printf("connection from %s, port %d\n",inet_ntop(AF_INET,&cliaddr.sin_addr,buff,sizeof(buff)),
		ntohs(cliaddr.sin_port));

		while((n=read(connfd,buff,MAXLINE))>0)
		{
			buff[n] = 0;
			if(fputs(buff,stdout)==EOF)
				err_sys("fputs error");

			sscanf(buff,"%s %s %s",cmd,path,vers);
			printf("%s %s %s",cmd,path,vers);
	
			if(strstr(buff,"\r\n\r\n")!=0)
				break;
		}

		if(strcmp(path,"/index.html")==0)
		{
			snprintf(buff,sizeof(buff),"<h1>Success</h1>\r\n");
		}
		else
		{
			snprintf(buff,sizeof(buff),"Failure\r\n");
		}

		Write(connfd,buff,strlen(buff));

		
		Close(connfd);
	}
}

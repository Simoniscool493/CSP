#include "unp.h"
#include <time.h>

int main(int argc,char **argv)
{
	int listenfd,connfd,n;
	struct sockaddr_in servaddr;
	char inBuff[MAXLINE];
	char outBuff[MAXLINE];
	int readingnumber = 0;
	
	//FILE * file;

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
		connfd = Accept(listenfd,(SA *) NULL,NULL);



		while((n=read(connfd,inBuff,MAXLINE))>0)
		{
			char junk[10];
			sscanf(inBuff,"%s %d",junk,readingnumber);
		}


		snprintf(outBuff,sizeof(outBuff),"Readingnumber is %d",readingnumber);

		Write(connfd,outBuff,strlen(outBuff));		
		Close(connfd);
	}
}

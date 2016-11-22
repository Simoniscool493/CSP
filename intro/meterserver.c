//Simon O'Neill C14444108

#include "unp.h"
#include <time.h>

int main(int argc,char **argv)
{
	int listenfd,connfd;
	struct sockaddr_in servaddr;
	char inBuff[MAXLINE];
	char outBuff[MAXLINE];
	int readingnumber;
	
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
		char junk[10];
		char number[10];

		connfd = Accept(listenfd,(SA *) NULL,NULL);

		read(connfd,inBuff,MAXLINE);
		

		sscanf(inBuff,"%s %s",junk,number);
		readingnumber = atoi(number);
		
		snprintf(outBuff,sizeof(outBuff),"Units: %d. Cost: %.2f\n",readingnumber,(((float)readingnumber)*1.3));
		Write(connfd,outBuff,strlen(outBuff));		


		printf("Written %s\n",outBuff);
		fflush(stdout);
		Close(connfd);
	}
}

#include "unp.h"
#include <time.h>

int main(int argc,char **argv)
{
	int listenfd,connfd,n;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
	char cmd[16];
 	char path[64];
	char path1[64] = ".";
	char vers[16];

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

		while((n=read(connfd,buff,MAXLINE))>0)
		{
			buff[n] = 0;
			if(fputs(buff,stdout)==EOF)
			{
				err_sys("fputs error");
			}

			sscanf(buff,"%s %s %s",cmd,path,vers);
			printf("%s %s %s\n",cmd,path,vers);

			strcat(path1,path);

			char filebuffer[100];

			FILE * file;

			printf("Opening file at path %s\n",path1);

			file = fopen(path1,"r");

			printf("File opened.");

			if(file)
			{
				printf("File found");

				while((fscanf(file,"%s",filebuffer)!=EOF))
				{
					Write(connfd,filebuffer,strlen(filebuffer));
				}

				fclose(file);
				break;
			}
			else
			{
				printf("File not found");
			}

			if(strstr(buff,"\r\n\r\n")!=0)
				break;
		}

		printf("Exited loop\n");

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

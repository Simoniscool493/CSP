#include "unp.h"

int main(int argc,char **argv)
{
	//arg1 is ip address, arg2 is port, arg3 is mail to, arg4 is mail content 
	int sockfd,n;
	char recvline[MAXLINE+1];
	struct sockaddr_in servaddr;
	char buff[MAXLINE];

	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		err_sys("socket error");
	}

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2])); /*daytime server port taken from the command line*/

	if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr)<=0)/*convert dotted decimal IP address to network byte ord*/
	{
		err_quit("inet_pton error for %s",argv[1]);
	}

	if(connect(sockfd,(SA *) &servaddr, sizeof(servaddr)) < 0)
	{
		err_sys("connect error");
	}



	n=read(sockfd,recvline,MAXLINE);
	printf("%s",recvline);

	snprintf(buff,sizeof(buff),"HELO attu.ict.ad.dit.ie\r\n");
	Write(sockfd,buff,strlen(buff));

	n=read(sockfd,recvline,MAXLINE);
	printf("%s",recvline);


	snprintf(buff,sizeof(buff),"MAIL FROM: <emailclient@mymail.ie>\r\n");
	Write(sockfd,buff,strlen(buff));

	n=read(sockfd,recvline,MAXLINE);
	printf("%s",recvline);


	snprintf(buff,sizeof(buff),"RCPT TO: <%s>\r\n",argv[3]);
	Write(sockfd,buff,strlen(buff));

	n=read(sockfd,recvline,MAXLINE);
	printf("%s",recvline);

	snprintf(buff,sizeof(buff),"DATA\r\n");
	Write(sockfd,buff,strlen(buff));

	n=read(sockfd,recvline,MAXLINE);
	printf("%s",recvline);

	snprintf(buff,sizeof(buff),"From: emailclient@mymail.ie\r\nTo: %s\r\nSubject: Test\r\n%s\r\n.\r\n",argv[3],argv[4]);
	Write(sockfd,buff,strlen(buff));

	n=read(sockfd,recvline,MAXLINE);
	printf("%s",recvline);

	snprintf(buff,sizeof(buff),"QUIT\r\n");
	Write(sockfd,buff,strlen(buff));

	n=read(sockfd,recvline,MAXLINE);
	printf("%s",recvline);


	/*while((n=read(sockfd,recvline,MAXLINE))>0)
	{
		recvline[n] = 0;
		if(fputs(recvline,stdout)==EOF)
			err_sys("fputs error");
	}*/

	if(n<0)
		err_sys("read error");


	exit(0);
}

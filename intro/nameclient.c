#include "unp.h"

int main(int argc,char **argv)
{
	int sockfd,n;
	char recvline[MAXLINE+1];
	struct sockaddr_in servaddr;
	char buff[MAXLINE];

	struct hostent *hptr;
	char **pptr;
	char str[INET_ADDRSTRLEN];

	hptr = gethostbyname(argv[1]);
	pptr = hptr->h_addr_list;

	Inet_ntop(hptr->h_addrtype,*pptr,str,sizeof(str));
	
	//if(argc!=3)
	//	err_quit("usage: a.out <IPaddress>");

	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
		err_sys("socket error");

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2])); /*daytime server port taken from the command line*/

	if(inet_pton(AF_INET,str,&servaddr.sin_addr)<=0)/*convert dotted decimal IP address to network byte ord*/
		err_quit("inet_pton error for %s",str);

	if(connect(sockfd,(SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	snprintf(buff,sizeof(buff),"GET /index.html HTTP/1.1\r\nHost: comp.dit.ie\r\nConnection: close\r\n\r\n");

	Write(sockfd,buff,strlen(buff));

	while((n=read(sockfd,recvline,MAXLINE))>0)
	{
		recvline[n] = 0;
		if(fputs(recvline,stdout)==EOF)
			err_sys("fputs error");
	}

	if(n<0)
		err_sys("read error");


	exit(0);
}

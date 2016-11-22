//Simon O'Neill C14444108

#include "unp.h"
#include <time.h>

int main(int argc,char **argv)
{
	int listenfd,connfd;
	struct sockaddr_in servaddr;
	char inBuff[MAXLINE];
	char outBuff[MAXLINE];
	//Define an inBuffer to read data, and an outBuffer to write data
	int readingnumber;
	//Define a readingnumber to hold the meter number

	if(argc!=2)
	{
		//check if syntax is correct and quit if not
		err_quit("usage: <Program Name> <Port No.>");
	}

	listenfd = Socket(AF_INET,SOCK_STREAM,0);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	Bind(listenfd,(SA *) &servaddr,sizeof(servaddr));
	Listen(listenfd,LISTENQ);
	//Receive port from argument passed in, bind, and start listening on it


	for( ; ; )//Start Infinite Loop 
	{
		char junk[10];
		char number[10];
		//Define two buffers, number and junk, to parse the code from the client 

		connfd = Accept(listenfd,(SA *) NULL,NULL);
		//Accept a connection from the client

		read(connfd,inBuff,MAXLINE);
		//Read in the line sent by the client into the inBuff

		sscanf(inBuff,"%s %s",junk,number);
		//Parse the data received by input into junk and number. Disregard junk
		readingnumber = atoi(number);
		//Call atoi to turn the string, number into the int, readingnumber
		
		snprintf(outBuff,sizeof(outBuff),"Units: %d. Cost: %.2f\n",readingnumber,(((float)readingnumber)*1.3));
		//Write the meter number and the cost to the outBuffer
		Write(connfd,outBuff,strlen(outBuff));	
		//Send the outBuffer to the client			


		printf("Written %s\n",outBuff);
		//Output the sent data to the console
		fflush(stdout);
		//Flush the input
		Close(connfd);
		//Close the connection, ready to receive another
	}//End Infinite Loop
}

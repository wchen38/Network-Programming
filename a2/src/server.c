//#include	"unp.h"
//#include	<time.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h>  
#include <stdio.h>
#include <sys/types.h>  /* basic system data types */
#include <sys/socket.h> /* basic socket definitions */
#include <sys/time.h>   /* timeval{} for select() */
#include <time.h>       /* timespec{} for pselect() */
#include <netinet/in.h> /* sockaddr_in{} and other Internet defns */

int 
main(int argc, char **argv)
{
	int 				listenfd = 0, connfd, n;
	struct sockaddr_in 	servaddr;
	unsigned short		port;
	char 				clientBuff[129];

	//get the port value from stdin, else default 5000
	port = (argc > 1) ? atoi(argv[1]) : 5000;

	//create socket
	if(( n = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
	}else {
		listenfd = n;
	}

	bzero(&servaddr, sizeof(servaddr)); //clear the servaddr
	servaddr.sin_family			= AF_INET; //set it to IPv4
	servaddr.sin_addr.s_addr 	= htonl(INADDR_ANY); 
	servaddr.sin_port			= htons(port); // set the port number

	/*set the socket(listenfd) as a passive listening socket,so the socket can start
 	*recive connection */
	if(bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        	perror("bind error");
       		 exit(1);
	}

		/*set the socket(listenfd) as a passive listening socket,so the socket can start
 	*recive connection */
	if(listen(listenfd, 7) < 0 ) {
           perror("listen error");
           exit(1);
	}

	for(;;) {
		/*once you recived the connecting, accept() returns socket descirptor*/
		connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);
		if(connfd < 0) { 
   	   		printf("er%d\n", connfd);
			perror("accept error");
	   		exit(1);
		}

		if(read(connfd, clientBuff, sizeof(clientBuff)-1) <0) {
			perror("read_error");
		}
		printf("clientBuff: %s\n", clientBuff);
		close(connfd);
	}




}
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
	int			listenfd, connfd, n, j,nbytes;
	struct sockaddr_in	servaddr;
	char			buff[128+1], str[] = "exit\n";
        unsigned short          port;      
        char 			endOfMessage[] = "endOfMessage";
        ssize_t 		bytes_read;

        FILE *in;
        extern FILE *popen();

 
        port = (argc > 1) ? atoi(argv[1]) : 5000;

  	if((n = socket(AF_INET, SOCK_STREAM, 0)) < 0) { //creates a socket of ipv4
		perror("socket error");
	}else {
        	listenfd = n;
	}
	bzero(&servaddr, sizeof(servaddr));//clear the servaddr
	servaddr.sin_family      = AF_INET; //set it to IPv4
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(port);	//set the port number
	
	/*assigns an ip adress to socket*/
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

	for ( ; ; ) {
		/*once you recived the connecting, accept() returns socket descirptor*/
		connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);
		if(connfd < 0) { 
   	        	printf("er%d\n", connfd);
			perror("accept error");
	   		exit(1);
		}


	/**********
 	*read()
	***************/
        	bytes_read = read(connfd, buff, sizeof(buff)-1);  
		if(bytes_read <0) {
		perror("read_error");
		}     
		
		nbytes = strlen(buff);
		//checking for exit command
		if((j =strncmp(str, buff, nbytes)) == 0) {
		
			printf("buff: %s\n", buff);
			close(connfd);
		//	exit(1);
		}
		
        	if(!(in = popen(buff, "r"))) {
                	exit(1);
        	}
		//each time it gets the output, it will send it to client
        	while (fgets(buff, sizeof(buff)-1, in) != NULL ) {
			write(connfd, buff, sizeof(buff)-1); //send it back to client
        	}
		//send a string to client to tell it the process is over
 		write(connfd, endOfMessage, sizeof(endOfMessage));
		
	}
}

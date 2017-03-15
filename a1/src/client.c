#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>  /* basic system data types */
#include <sys/socket.h> /* basic socket definitions */
#include <sys/time.h>   /* timeval{} for select() */
#include <time.h>       /* timespec{} for pselect() */
#include <netinet/in.h> /* sockaddr_in{} and other Internet defns */
#include <netdb.h>      /* needed by gethostbyname */
#include <arpa/inet.h>  /* needed by inet_ntoa */
//#include </~/Wuyuan/Desktop/WINTER2017/CE156/unpv13e/lib/error.c>
int
main(int argc, char **argv)
{
	int			sockfd, n; //socket descriptor
	char			recvline[128 + 1], buffer[129], str[] = "exit\n";
 	char			endOfMessage[] = "endOfMessage";
	struct sockaddr_in	servaddr;
        unsigned short        port;
        size_t nbytes;
	

	if (argc < 3){
		perror("usage: a.out <IPaddress>");
		exit(0);
	}
	//if no second argument then default port is 5000
	port = (argc > 2) ? atoi(argv[2]) : 5000;
	printf("Welcome to my shell!\n");
	printf("please type in the following command:\n");
	printf("cat /etc/resolv.conf\n");
	printf("cat <filename>\n");
	printf("date\n");
	printf("df -h\n");
	printf("du\n");
	printf("echo $PATH\n");
	printf("echo <environment variable>");
	printf("free\nlast\npsaux\npwd\nset\ntty\nuname -a\n");
	printf("uptime\nwho\nexit\n");

	printf("------------------------------------------------\n");
			
	for(;;){
		if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ //set up TCP and returns a file descriptor
			perror("socket error");
			exit(1);
		}

		bzero(&servaddr, sizeof(servaddr)); //set servaddr to zero
		servaddr.sin_family = AF_INET; //set address family to IPV4
		servaddr.sin_port   = htons(port);	/* daytime server */
		
		if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){ //convert IP address to binary
		perror("inet_pton error for");
		exit(1);
		}

		if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {//Call connect() to setup a TCP connection with the server, and check the return value
		perror("connect error");
		exit(1);
		}

		/***************************************
 		*scan in stdin 
		********************************** ****/
		printf("> ");
		fgets(buffer, sizeof(buffer), stdin);
        	nbytes = strlen(buffer);
		//checking for string exit
        	if(strncmp(str, buffer, nbytes) == 0) {
			write(sockfd, buffer, sizeof(buffer));
			close(sockfd);
        		exit(1);
        	}
		/***************************************
 		**scan in stdin
		 ***************************************/
		write(sockfd, buffer, sizeof(buffer)); //store buffer to sockfd then send to server
 	
		while ( (n = read(sockfd, recvline, sizeof(recvline)-1)) > 0) { //read up to MAXLINE, transfer sockfd to recvline until nothing more to read
			recvline[n] = 0;	/* null terminate */
      			//allow the client to tell server that it reached end of file and break out of while loop so it can start a new command.	
//			assert(strcmp(recvline, endOfMessage) == 1); 
			if(strcmp(recvline, endOfMessage) == 0) {
      				break;
                        }
			
			//prints out the responds from server to stdout
			if (fputs(recvline, stdout) == EOF){
				perror("fputs error");
				exit(1);
			}
		}
		if (n < 0){
			perror("read error");
			exit(1);
		}
		close(sockfd);
	}
}

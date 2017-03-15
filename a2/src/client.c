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
#include <sys/stat.h>
#include <pthread.h>

//functions
void parse(char *line);
void *thread_func(void *);

//golobal variables 
//char 				ipAdress[]="", portNum[] = "";
char 				*addrTok, *portTok, *addrTok2;
pthread_mutex_t 	mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t 	mutex2 = PTHREAD_MUTEX_INITIALIZER;
int 				counter =0;
unsigned short		port, addrNum;

int main(int argc, char **argv)
{
	int 			fileSize, i, j;
	char 			*file, inputLine[60], *textFile;
	FILE 			*fp;
	unsigned short	numCons;
	struct stat 	st;
	pthread_t 		thread_id[3]; 

	if(argc <4) {
		perror("usage: a.out <IPaddress>");
		exit(0);
	}

	//get the number of connections
	numCons = atoi(argv[2]);
	file = argv[1];
	//get the file size	
	textFile = argv[3];
	stat(textFile, &st);
	fileSize = st.st_size;

	
	printf("filesize %d!\n", fileSize);
	fp = fopen(file,"r");
	if(fp) {
		i=0, j=0;
		//create multi-threads
			
		while(1) {
			pthread_mutex_lock(&mutex2);
			printf("Welcome to my Client %d!\n", counter);
			//gets each line of the input file
			if(fgets(inputLine, 60, fp) != NULL) {
				i++;
				/*pthread_mutex_lock() waits for pthread_create() to finish
			 	* before moving on to create the next thread*/
				parse(inputLine);
				pthread_create(&thread_id[i], NULL, thread_func, NULL);
			}
		}
		//block each threads, so it can execute one at a time
		while(j<4) {
			j++;
			pthread_join( thread_id[j], NULL); 
		}	
		fclose(fp);
	}

	
}

//parses each line into address token and port token
void parse(char *line) {
	addrTok = strtok(line, " ");
	portTok = strtok(NULL, "\n");
	port = atoi(portTok);
	
}

void *thread_func(void *dummyPtr)
{	
	char* 				threadAddr = addrTok;
	char 				str[] = "hello\n";
	int 				threadPort = port;
	int 				sockfd;
	struct sockaddr_in 	servaddr;
	counter++;

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ //set up TCP and returns a file descriptor
		perror("socket error");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr)); //set servaddr to zero
	servaddr.sin_family = AF_INET; //set address family to IPV4
	servaddr.sin_port   = htons(threadPort);	/* daytime server */

	if (inet_pton(AF_INET, threadAddr, &servaddr.sin_addr) <= 0){ //convert IP address to binary
		perror("inet_pton error for");
		exit(1);
	}

	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {//Call connect() to setup a TCP connection with the server, and check the return value
		perror("connect error");
		exit(1);
	}
	//printf("str before client write: %s\n", str);
	write(sockfd, str, strlen(str));

	/*pthread finishes creating the thread so it can be 
	 *released from the lock statement.	*/
	pthread_mutex_unlock(&mutex2);
//	return 1;
	
}


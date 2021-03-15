#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <errno.h>
#include <pthread.h>

#define PORT 4000

void* routine(void *);

int main(int argc, char *argv[])
{
	int *sock;
	int sockfd, newsockfd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
        printf("ERROR opening socket");
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);     
    
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		printf("ERROR on binding");
	
	listen(sockfd, 5);
	
	clilen = sizeof(struct sockaddr_in);

	pthread_t thr;
	while(1)
	{

		if( (newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1)
			printf("ERROR on accept");
		else
		{
			sock = (int *)malloc(sizeof(int));
			*sock = newsockfd;
			pthread_create(&thr, NULL, &routine, (void *)sock);
		}

	}

	close(sockfd);
	exit(1);
}

void* routine(void * arg)
{
	int newsockfd = *(int*)arg;
	free(arg);

	printf("%d\n", newsockfd);

	int n;
	char buffer[256];

	bzero(buffer, 256);

	while(1)
	{
		/* read from the socket */
		n = read(newsockfd, buffer, 256);
		if (n < 0)
		{
			printf("ERROR reading from socket");
			break;
		}

		n = write(newsockfd,buffer,n);
		if (n < 0)
		{
			printf("ERROR writing to socket");
			break;
		}

		if(buffer[0] == 'a')
			break;
	}

	close(newsockfd);
	pthread_exit((void *)1);
}
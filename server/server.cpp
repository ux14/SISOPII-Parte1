#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <errno.h>
#include <pthread.h>

#include <bits/stdc++.h>
#include "socketUser.h"
#include "user_controller.h"

#define PORT 4000

using namespace std;

void *routine(void *);

vector<struct socketUser> sessions;

string getCommand(string message)
{
	int position = message.find("|");
	return message.substr(0, position);
}

string getMessage(string message)
{
	int position = message.find("|");
	return message.substr(position + 1);
}

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

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		printf("ERROR on binding");

	listen(sockfd, 5);

	clilen = sizeof(struct sockaddr_in);

	pthread_t thr;
	while (1)
	{

		if ((newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen)) == -1)
			printf("ERROR on accept");
		else
		{
			sock = (int *)malloc(sizeof(int));
			*sock = newsockfd;
			pthread_create(&thr, NULL, &routine, (void *)sock);

			struct socketUser usuario;
			usuario.socketId = newsockfd;
			usuario.thread = thr;
			usuario.user = "";

			sessions.push_back(usuario);
			cout << "Session size " << sessions.size() << endl;
		}
	}

	close(sockfd);
	exit(1);
}

void *routine(void *arg)
{
	int newsockfd = *(int *)arg;
	free(arg);

	printf("%d\n", newsockfd);

	int n;
	char buffer[256];

	bzero(buffer, 256);

	//Receber dados login
	n = read(newsockfd, buffer, 256);
	string user = string(buffer);
	cout << user << endl;
	user = getMessage(user);
	UserController userController(&sessions);

	if (userController.login(user))
	{
		//Atrelar socket ao usuário
		//cout << sessions.size() << endl;
		for (int i = 0; i < sessions.size(); i++)
		{
			if (sessions[i].socketId == newsockfd)
			{
				sessions[i].user = user;
				break;
			}
		}

		while (1)
		{
			bzero(buffer, 256);
			/* read from the socket */
			n = read(newsockfd, buffer, 256);
			if (n < 0)
			{

				printf("ERROR reading from socket");
				break;
			}
			string command = getCommand(string(buffer));
			string message = getMessage(string(buffer));
			if (command == "SEND")
			{
				//Acontence algo com as mensagens
			}
			else if (command == "FOLLOW")
			{
				userController.follow(user, message);
			}
			else if (command == "LOGOUT")
			{
				break;
			}			

			string msg = "SEND##mensagem teste##test_user";

			n = write(newsockfd, msg.c_str(), msg.length());
			if (n < 0)
			{
				printf("ERROR writing to socket");
				break;
			}

			if (buffer[0] == 'a')
				break;
		}
	}
	close(newsockfd);
	pthread_exit((void *)1);
}
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
#include <thread>
#include <pthread.h>

#include <bits/stdc++.h>
#include "socketUser.h"
#include "user_controller.h"
#include "notification_controller.h"

#define PORT 4000

using namespace std;

void read_job(int fd, user_t user, UserController *user_controller, NotificationController *notification_controller);

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

	UserController user_controller;
	NotificationController notification_controller(&user_controller);
	
	std::thread producer(&NotificationController::producerThread,&notification_controller);
	producer.detach();

	while (1)
	{

		if ((newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen)) == -1)
			printf("ERROR on accept");
		else
		{
				int n;
				char buffer[256];

				bzero(buffer, 256);

				//Receber dados login
				n = read(newsockfd, buffer, 256);
				string user = string(buffer);
				cout << user << endl;
				user = getMessage(user);

				if (user_controller.login(user))
				{
					std::thread reader(&read_job,newsockfd,user,&user_controller,&notification_controller);
					reader.detach();

					struct socketUser usuario;
					usuario.socketId = newsockfd;
					usuario.user = user;

					user_controller.registerSession(usuario);

					std::thread consumer(&NotificationController::consumerThread,&notification_controller,user);
					consumer.detach();
				}
				else
				{
					//logout por ter mais de 2 sessões
				}
		}
	}

	close(sockfd);
	exit(1);
}

void read_job(int sockfd, user_t user, UserController *user_controller, NotificationController *notification_controller)
{
	char buffer[256];
	bzero(buffer, 256);

	int n;

	while(1)
	{
		/* read from the socket */
		n = read(sockfd, buffer, 256);
		if (n <= 0)
		{
			if(n < 0)
				printf("ERROR reading from socket\n");
			close(sockfd);
			break;
		}

		string command = getCommand(string(buffer));
		string message = getMessage(string(buffer));

		cout << "comando: " << command << ' ' << "mensagem: "<< message << '\n';
		if (command == "SEND")
		{
			notification n;
			std::copy(message.begin(), message.end(), n.msg);
			n.length = message.length();
			//Acontence algo com as mensagens
			notification_controller->producerQueue.push(std::make_pair(user,n));
		}
		else if (command == "FOLLOW")
		{
			user_controller->follow(user, message);
		}
		else if (command == "LOGOUT")
		{
			close(sockfd);
			break;
		}

		printf("Here is the message: %s\n", buffer);
	}
}
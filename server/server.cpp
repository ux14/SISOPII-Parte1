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
#include "input_controller.h"

#define PORT 4000

using namespace std;

int main(int argc, char *argv[])
{
	int sockfd, newsockfd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		printf("ERROR opening socket\n");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		printf("ERROR on binding\n");

	listen(sockfd, 5);

	clilen = sizeof(struct sockaddr_in);

	UserController user_controller;
	NotificationController notification_controller(&user_controller);
	InputController input_controller(&user_controller, &notification_controller);

	// para todos users já registrados, começar consumer thread
	for ( auto u : user_controller.getUsers() )
	{
		std::thread consumer(&NotificationController::consumerThread,&notification_controller,u);
		consumer.detach();
	}

	std::thread producer(&NotificationController::producerThread,&notification_controller);
	producer.detach();

	while (1)
	{

		if ((newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen)) == -1)
			printf("ERROR on accept\n");
		else
		{
				int n;
				char buffer[256];

				bzero(buffer, 256);

				//Receber dados login
				n = read(newsockfd, buffer, 256);
				if(n <= 0)
				{
					if(n < 0)
						printf("ERROR on read\n");
					else
						printf("No login attempt\n");
					
					close(newsockfd);
					continue;
				}
				
				string user = string(buffer);
				cout << user << endl;
				user = input_controller.getMessage(user);
				cout << user << endl;
				
				// apenas começa uma consumer se o usuário não existir
				if (!user_controller.userExists(user))
				{
					std::thread consumer(&NotificationController::consumerThread,&notification_controller,user);
					consumer.detach();
				}

				if (user_controller.login(user))
				{
					std::thread reader(&InputController::read_job,&input_controller,newsockfd,user);
					reader.detach();

					struct socketUser usuario;
					usuario.socketId = newsockfd;
					usuario.user = user;

					user_controller.registerSession(usuario);
				}
				else
				{
					//logout por ter mais de 2 sessões
					string error = "ERROR##User has hit session limit##" + user;
					n = write(newsockfd, error.c_str(), error.size());
					sleep(2);
					close(newsockfd);
				}
		}
	}

	close(sockfd);
	exit(1);
}

